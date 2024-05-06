#include "raylib.h"
#include "raymath.h"        // Required for: Vector3, Quaternion and Matrix functionality
#include "utils.h"          // Required for: TRACELOG(), LoadFileData(), LoadFileText(), SaveFileText()

#include <cassert>
#include <FileHandler.h>
#include "../libvitaboy/libvitaboy.hpp"


#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#define WINDOW_TITLE "libvitaboy - Renderer - Ray"

//we dont look in this mess for now
#pragma region TSO
//util
static bool Read(const char* Filename, uint8_t** InData) {
    *InData = File::ReadFile(Filename);
    if (*InData != NULL) {
        VBFile.set(*InData, File::FileSize);
        return true;
    }
    return false;
}

//globals
//skeleton
static Skeleton_t Skeleton;
static Model box_model;
static void DrawBonesSkeleton(Bone_t& Bone, const Matrix& M)
{
    Vector3 axis{ Vector3Zero()};
    float angle{ 0 };
    const Quaternion rotation = Quaternion{ Bone.Rotation.x, Bone.Rotation.y, Bone.Rotation.z, Bone.Rotation.w };
    QuaternionToAxisAngle(rotation, &axis, &angle);

    // A normalized quaternion(called a "versor") consisting of X, Y, Z, W coordinates, each a 32 - bit little - endian float, 
    // specifying the default rotation of this bone to be applied after translation.
    Matrix matRotation = MatrixRotate(axis, angle * DEG2RAD);
    // X,Y,Z coordinates, each a 32-bit little-endian float, 
    // specifying the distance from the joint with the parent to the distal end of this bone after the default rotation has been applied
    Matrix matTranslation = MatrixTranslate(Bone.Translation.x, Bone.Translation.y, Bone.Translation.z);
    
    Matrix matTransform = MatrixMultiply(matTranslation, matRotation);
    Matrix modelMatrix = MatrixMultiply(M, matTranslation);

    Color color;
    if(!strcmp(Bone.Name, "ROOT"))
    {
        color = RED;
    }
    else if(!strcmp(Bone.Name, "HEAD"))
    {
        color = YELLOW;
    }
    else
    {
        color = GREEN;
    }
    const Vector3 position{ modelMatrix.m12, modelMatrix.m13, modelMatrix.m14 };
    DrawModel(box_model, position, 1.f, color);
    
    if (Bone.ChildrenCount == 1)
    {
        DrawBonesSkeleton(*Bone.Children[0], M);
    }
    else if (Bone.ChildrenCount > 1)
    {
        for (unsigned i = 0; i < Bone.ChildrenCount; i++)
        {
            DrawBonesSkeleton(*Bone.Children[i], modelMatrix);
        }
    }
}

static int counter = 0;
static void DrawTest(const Matrix& M)
{
    Vector3 scale = { 1.f, 1.f, 1.f };
    Vector3 rotationAxis = { 0.0f, 0.0f, 1.0f };
    Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
    Matrix matRotation = MatrixRotate(rotationAxis, 45.f * DEG2RAD);
    Matrix matTranslation = MatrixTranslate(5.f, 0.f, 0.f); //transform 5 in the X direction

    // https://github.com/JipBoesenkool/CSE167F17_Project4/blob/master/src/renderer/model/Transform.cpp
    // SRT = iTRS, for absolute
    // STR = iRTS, for local transforms
    Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matTranslation), matRotation);
    Matrix modelMatrix = MatrixMultiply(matTransform, M);

    DrawModel (box_model, //root
        Vector3Transform(Vector3{ 0.f, 0.f, 0.f }, modelMatrix),
        1.f, YELLOW);
    DrawModel(box_model, //x
        Vector3Transform(Vector3{ 1.f, 0.f, 0.f }, modelMatrix),
        1.f, RED);
    DrawModel(box_model, //y
        Vector3Transform(Vector3{ 0.f, 1.f, 0.f }, modelMatrix),
        1.f, GREEN);
    DrawModel(box_model, //z
        Vector3Transform(Vector3{ 0.f, 0.f, 1.f }, modelMatrix),
        1.f, BLUE);
    

    if (counter < 3)
    {
        counter++;
        DrawTest(modelMatrix);
    }
    counter = 0;
}

//animation
static Animation_t Animation;
static float AnimationTime = 0;
static void AdvanceFrame(Skeleton_t& Skeleton, Animation_t& Animation, float TimeDelta)
{
    float Duration = (float)Animation.Motions[0].FrameCount/30;
    AnimationTime += TimeDelta;
    AnimationTime = fmodf(AnimationTime, Duration); //Loop the animation

    for(unsigned i=0; i<Animation.MotionsCount; i++)
    {
        unsigned BoneIndex = FindBone(Skeleton, Animation.Motions[i].BoneName, Skeleton.BoneCount);
        if(BoneIndex == (unsigned)-1) continue;

        Bone_t& Bone = Skeleton.Bones[BoneIndex];

        unsigned Frame = AnimationTime*30;
        float FractionShown = AnimationTime*30 - Frame;
        unsigned NextFrame = (Frame+1 != Animation.Motions[0].FrameCount) ? Frame+1 : 0;

        if(Animation.Motions[i].HasTranslation)
        {
            Translation_t& Translation = Animation.Motions[i].Translations[Frame];
            Translation_t& NextTranslation = Animation.Motions[i].Translations[NextFrame];
            Bone.Translation.x = (1-FractionShown)*Translation.x + FractionShown*NextTranslation.x;
            Bone.Translation.y = (1-FractionShown)*Translation.y + FractionShown*NextTranslation.y;
            Bone.Translation.z = (1-FractionShown)*Translation.z + FractionShown*NextTranslation.z;
        }
        if(Animation.Motions[i].HasRotation)
        {
            Rotation_t& Rotation = Animation.Motions[i].Rotations[Frame];
            Rotation_t& NextRotation = Animation.Motions[i].Rotations[NextFrame];

            //Use nlerp to interpolate
            float w1 = 1.0f - FractionShown, w2 = FractionShown;
            if(DotProduct(&Rotation, &NextRotation) < 0)
                w1 *= -1;

            Bone.Rotation.x = w1*Rotation.x + w2*NextRotation.x;
            Bone.Rotation.y = w1*Rotation.y + w2*NextRotation.y;
            Bone.Rotation.z = w1*Rotation.z + w2*NextRotation.z;
            Bone.Rotation.w = w1*Rotation.w + w2*NextRotation.w;

            Normalize(&Bone.Rotation);
        }
    }
}

//textures
static Texture2D textures[3];
enum { Texture_Body, Texture_Head, Texture_Hand };
static const char* const TexturePaths[] = {"body.jpg", "head.jpg", "hand.jpg"};

static bool LoadTextures()
{
    for(int i=0; i < 3; i++)
    {
        textures[i] = LoadTexture(TexturePaths[i]);
    }
    return true;
}

//meshes
static const unsigned MeshCount = 4;
static Mesh_t Meshes[4];
enum { Mesh_Body, Mesh_Head, Mesh_LHand, Mesh_RHand };
static const char* const MeshPaths[]    = {"body.mesh", "head.mesh", "lhand.mesh", "rhand.mesh" };
static const unsigned Mesh_UseTexture[] = { Texture_Body, Texture_Head, Texture_Hand, Texture_Hand };
static const char* const MeshActivate[] = {NULL, "HEAD", "L_HAND", "R_HAND"};

static bool LoadMeshes()
{
    uint8_t* InData;

    for (unsigned i = 0; i < MeshCount; i++) {
        if (!Read(MeshPaths[i], &InData))
        {
            return false;
        }
        ReadMesh(Meshes[i]);
        free(InData);
    }
    return true;
}

//do this once!
static void TransformVertices(Bone_t& Bone, const Matrix& M)
{
    Matrix matTranslation = MatrixTranslate(Bone.Translation.x, Bone.Translation.y, Bone.Translation.z);
    Vector3 axis{ Vector3Zero() };
    float angle{ 0 };
    const Quaternion rotation = Quaternion{ Bone.Rotation.x, Bone.Rotation.y, Bone.Rotation.z, Bone.Rotation.w };
    QuaternionToAxisAngle(rotation, &axis, &angle);
    Matrix matRotation = MatrixRotate(axis, angle * DEG2RAD);

    Matrix matTransform = MatrixMultiply(matTranslation, M);
    matTransform = MatrixMultiply(matRotation, matTransform);

    unsigned MeshIndex = 0;
    unsigned BoneIndex;

    for (unsigned i = 1; i < MeshCount; i++) {
        if (!strcmp(Bone.Name, MeshActivate[i])) {
            MeshIndex = i;
            break;
        }
    }
    Mesh_t& Mesh = Meshes[MeshIndex];
    for (BoneIndex = 0; BoneIndex < Mesh.BindingCount; BoneIndex++) {
        if (!strcmp(Bone.Name, Mesh.BoneNames[Mesh.BoneBindings[BoneIndex].BoneIndex]))
            break;
    }

    if (BoneIndex < Mesh.BindingCount)
    {
        for (unsigned i = 0; i < Mesh.BoneBindings[BoneIndex].RealVertexCount; i++)
        {
            unsigned VertexIndex = Mesh.BoneBindings[BoneIndex].FirstRealVertex + i;
            Vertex_t& RelativeVertex = Mesh.VertexData[VertexIndex];
            Vertex_t& AbsoluteVertex = Mesh.TransformedVertexData[VertexIndex];

            const Vector3 vRelativePos{ RelativeVertex.Coord.x, RelativeVertex.Coord.y, RelativeVertex.Coord.z };
            const Vector3 vAbsolutePos = Vector3Transform(vRelativePos, matTransform);

            AbsoluteVertex.Coord.x = vAbsolutePos.x;
            AbsoluteVertex.Coord.y = vAbsolutePos.y;
            AbsoluteVertex.Coord.z = vAbsolutePos.z;
        }
        for (unsigned i = 0; i < Mesh.BoneBindings[BoneIndex].BlendVertexCount; i++)
        {
            unsigned VertexIndex = Mesh.RealVertexCount + Mesh.BoneBindings[BoneIndex].FirstBlendVertex + i;
            Vertex_t& RelativeVertex = Mesh.VertexData[VertexIndex];
            Vertex_t& AbsoluteVertex = Mesh.TransformedVertexData[VertexIndex];

            const Vector3 vRelativePos{ RelativeVertex.Coord.x, RelativeVertex.Coord.y, RelativeVertex.Coord.z };
            const Vector3 vAbsolutePos = Vector3Transform(vRelativePos, matTransform);

            AbsoluteVertex.Coord.x = vAbsolutePos.x;
            AbsoluteVertex.Coord.y = vAbsolutePos.y;
            AbsoluteVertex.Coord.z = vAbsolutePos.z;
        }
    }

    if (Bone.ChildrenCount == 1)
    {
        TransformVertices(*Bone.Children[0], M);
    }
    else if (Bone.ChildrenCount > 1)
    {
        for (unsigned i = 0; i < Bone.ChildrenCount; i++)
        {
            TransformVertices(*Bone.Children[i], matTransform);
        }
    }
}

static void BlendVertices()
{
    for (unsigned i = 0; i < MeshCount; i++)
    {
        Mesh_t& Mesh = Meshes[i];
        for (unsigned i = 0; i < Mesh.BlendVertexCount; i++)
        {
            Vertex_t& BlendVertex = Mesh.TransformedVertexData[Mesh.RealVertexCount + i];
            float Weight = BlendVertex.BlendData.Weight;
            Vertex_t& RealVertex = Mesh.TransformedVertexData[BlendVertex.BlendData.OtherVertex];
            RealVertex.Coord.x =
                Weight * BlendVertex.Coord.x +
                (1 - Weight) * RealVertex.Coord.x;
            RealVertex.Coord.y =
                Weight * BlendVertex.Coord.y +
                (1 - Weight) * RealVertex.Coord.y;
            RealVertex.Coord.z =
                Weight * BlendVertex.Coord.z +
                (1 - Weight) * RealVertex.Coord.z;
        }
    }
}
#pragma endregion TSO

#pragma region custom_ray
namespace CustomRay
{
    static Model LoadModelTSO()
    {
        Model model;
        //meshes
        model.meshCount = MeshCount;
        model.meshes = (Mesh*)RL_CALLOC(model.meshCount, sizeof(Mesh));
        //textures
        model.materialCount = 3;
        model.meshMaterial = (int*)RL_CALLOC(model.meshCount, sizeof(int)); // Material index assigned to each mesh
        model.materials = (Material*)RL_CALLOC(model.materialCount, sizeof(Material));
        model.meshMaterial[0] = 0;  // By default, assign material 0 to each mesh

        ///load the textures
        for (int i = 0; i < model.materialCount; i++)
        {
            model.materials[i] = LoadMaterialDefault();
            model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = textures[i];
        }

        //load meshes
        for (int i = 0; i < model.meshCount; i++) 
        {
            Mesh_t& tso_mesh = Meshes[i];
            Mesh&   ray_mesh = model.meshes[i];

            ray_mesh.vertexCount = tso_mesh.RealVertexCount;
            ray_mesh.triangleCount = tso_mesh.FaceCount;   // Face count (triangulated)
            ray_mesh.vertices = (float*)RL_CALLOC(ray_mesh.vertexCount * 3, sizeof(float));
            ray_mesh.texcoords = (float*)RL_CALLOC(ray_mesh.vertexCount * 2, sizeof(float));
            ray_mesh.normals = (float*)RL_CALLOC(ray_mesh.vertexCount * 3, sizeof(float));
            ray_mesh.indices = (unsigned short*)RL_CALLOC(ray_mesh.triangleCount * 3, sizeof(unsigned short));

            // Process all mesh faces
            //vertex data
            for (unsigned j = 0; j < ray_mesh.vertexCount; j++)
            {
                //vertices?
                ray_mesh.vertices[j * 3 + 0] = tso_mesh.TransformedVertexData[j].Coord.x;
                ray_mesh.vertices[j * 3 + 1] = tso_mesh.TransformedVertexData[j].Coord.y;
                ray_mesh.vertices[j * 3 + 2] = tso_mesh.TransformedVertexData[j].Coord.z;
                //ray_mesh.vertices[j * 3 + 0] = tso_mesh.VertexData[j].Coord.x;
                //ray_mesh.vertices[j * 3 + 1] = tso_mesh.VertexData[j].Coord.y;
                //ray_mesh.vertices[j * 3 + 2] = tso_mesh.VertexData[j].Coord.z + (i * 1.f); //offset for now so we can see them
                //coords
                ray_mesh.texcoords[j * 2 + 0] = tso_mesh.TransformedVertexData[j].TextureCoord.u;
                ray_mesh.texcoords[j * 2 + 1] = -tso_mesh.TransformedVertexData[j].TextureCoord.v;
                //normals
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.x;
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.y;
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.z;
            }
            //indices
            for (unsigned j = 0; j < ray_mesh.triangleCount; j++)
            {
                ray_mesh.indices[j * 3 + 0] = (unsigned short)tso_mesh.FaceData[j].VertexA;
                //counter clock wise
                ray_mesh.indices[j * 3 + 1] = (unsigned short)tso_mesh.FaceData[j].VertexC;
                ray_mesh.indices[j * 3 + 2] = (unsigned short)tso_mesh.FaceData[j].VertexB;
                //Clock wise
                //ray_mesh.indices[j * 3 + 1] = (unsigned short)tso_mesh.FaceData[j].VertexB;
                //ray_mesh.indices[j * 3 + 2] = (unsigned short)tso_mesh.FaceData[j].VertexC;
            }
            //select the textures
            model.meshMaterial[i] = Mesh_UseTexture[i];
        }

        //upload to gpu
        // Make sure model transform is set to identity matrix!
        model.transform = MatrixIdentity();
        if ((model.meshCount != 0) && (model.meshes != NULL))
        {
            // Upload vertex data to GPU (static meshes)
            for (int i = 0; i < model.meshCount; i++) UploadMesh(&model.meshes[i], false);
        }
        else TRACELOG(LOG_WARNING, "MESH: [%s] Failed to load model mesh(es) data", fileName);

        if (model.materialCount == 0)
        {
            TRACELOG(LOG_WARNING, "MATERIAL: [%s] Failed to load model material data, default to white material", fileName);

            model.materialCount = 1;
            model.materials = (Material*)RL_CALLOC(model.materialCount, sizeof(Material));
            model.materials[0] = LoadMaterialDefault();

            if (model.meshMaterial == NULL)
            {
                model.meshMaterial = (int*)RL_CALLOC(model.meshCount, sizeof(int));
            }
        }

        return model;
    }

    ModelAnimation LoadModelAnimationsTSO()
    {
        ModelAnimation result;
        // Translation data - For each translation:
        // Translation - X, Y, Z coordinates, each a 32 - bit little - endian float, specifying new, 
        // REPLACEMENT translation values for the bones of the skeleton.These values are stored absolute.

        // Rotation data - For each rotation :
        // Rotation - A normalized quaternion(called a "versor") consisting of X, Y, Z, W coordinates, each a 32 - bit little - endian float, specifying new, 
        // replacement rotation values for the bones of the skeleton.These values are stored absolute.
        // These values assume a left - handed coordinate system, meaning that for a right - handed coordinate system(like OpenGL and XNA), you have to mirror the quaternion by negating Y and Z.
        return result;
    }

    //https://gist.github.com/Gamerfiend/18206474679bf5873925c839d0d6a6d0
    void LoadSkeletonTSO(Model& ray_model)
    {
        //Load the bonus
        // map string to ID
        Skeleton_t& tso_skeleton = Skeleton;
        const unsigned int boneCount = Skeleton.BoneCount;
        ray_model.boneCount = boneCount;
        ray_model.bones = (BoneInfo*)RL_MALLOC(boneCount * sizeof(BoneInfo));
        ray_model.bindPose = (Transform*)RL_MALLOC(boneCount * sizeof(Transform));

        for (unsigned int i = 0; i < Skeleton.BoneCount; i++)
        {
            Bone_t& tso_bone = Skeleton.Bones[i];

            BoneInfo& ray_bone = ray_model.bones[i];
            Transform& ray_bone_transform = ray_model.bindPose[i];

            //fill boneinfo
            //sims naming might be bigger then 32 chars, assert if so
            const int length = strlen(tso_bone.Name);
            assert(strlen(tso_bone.Name) <= 32);
            strcpy(ray_bone.name, tso_bone.Name);
            ray_bone.parent = FindBone(Skeleton, tso_bone.ParentsName, boneCount);

            printf("Bone: %i\n", i);
            printf("Name: %s\n", ray_bone.name);
            printf("parentName: %s\n", tso_bone.ParentsName);
            printf("parentID: %i\n", ray_bone.parent);

            // Set the transform
            const Translation_t& tso_bone_position = tso_bone.Translation;
            const Rotation_t& tso_bone_rotation    = tso_bone.Rotation;
            const Vector3 tso_bone_Scale           = { 1.f, 1.f, 1.f, }; //no scale?
            // sucks writing it out, but safer then pointer casting
            //position
            ray_bone_transform.translation.x = tso_bone_position.x;
            ray_bone_transform.translation.y = tso_bone_position.y;
            ray_bone_transform.translation.z = tso_bone_position.z;
            //rotation
            ray_bone_transform.rotation.x = tso_bone_rotation.x;
            ray_bone_transform.rotation.y = tso_bone_rotation.y;
            ray_bone_transform.rotation.z = tso_bone_rotation.z;
            ray_bone_transform.rotation.w = tso_bone_rotation.w;
            //scale
            ray_bone_transform.scale.x = tso_bone_Scale.x;
            ray_bone_transform.scale.y = tso_bone_Scale.y;
            ray_bone_transform.scale.z = tso_bone_Scale.z;
        }

        for (int i = 0; i < ray_model.meshCount; i++)
        {
            Mesh_t& tso_mesh = Meshes[i];
            Mesh& ray_mesh = ray_model.meshes[i];
        }

        [[maybe_unused]] unsigned int block = 0;
        block++;
    }
}
#pragma endregion custom_ray

static int Startup()
{
    uint8_t * InData;
    if(!Read("skeleton.skel", &InData))
        return 0;
    ReadSkeleton(Skeleton);
    free(InData);

    for(unsigned i=0; i<MeshCount; i++){
        if(!Read(MeshPaths[i], &InData))
            return 0;
        ReadMesh(Meshes[i]);
        free(InData);
    }

    if(!Read("animation.anim", &InData))
        return 0;
    ReadAnimation(Animation);
    free(InData);

    AdvanceFrame(Skeleton, Animation, 0);
    return 1;
}

//settings
static bool ShowTextures = false;
static bool ShowMesh = true;
static bool ShowSkeleton = true;
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    // Initialize the camera
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.f, 5.0f, 5.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };  // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };      // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                          // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;       // Camera projection type - we will probably need orthographic

    //doesn't work when ran from VSStudio
    assert( Startup() );
    assert( LoadTextures() );
    assert( LoadMeshes() );

    printf("======================================\n");
    printf("=================RAY==================\n");
    printf("======================================\n");

    const Matrix& matIdentity = MatrixIdentity();
    const float size = 0.1f;
    box_model = LoadModelFromMesh( GenMeshCube(size, size, size) );
    
    //do this once!
    TransformVertices(Skeleton.Bones[0], matIdentity);
    BlendVertices();

    Model model = CustomRay::LoadModelTSO();
    CustomRay::LoadSkeletonTSO(model);

    DisableCursor();

    while (!WindowShouldClose())
    {
        const float dt{ GetFrameTime() };
        //update
        {
            UpdateCamera(&camera, CAMERA_FREE);
            // Play animation when spacebar is held down
            if (IsKeyDown(KEY_N))
            {
                AdvanceFrame(Skeleton, Animation, dt);
            }
            if (IsKeyPressed(KEY_ONE))
            {
                ShowSkeleton = !ShowSkeleton;
            }
            if (IsKeyPressed(KEY_TWO))
            {
                ShowMesh = !ShowMesh;
            }
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);
            BeginMode3D(camera);
            {
                DrawGrid(10, 5.0f);

                if(ShowSkeleton)
                {
                    DrawBonesSkeleton(Skeleton.Bones[0], matIdentity);
                }

                if (ShowMesh)
                {
                    const Vector3 position{ 0.0f, 0.0f, 0.0f }; // Set model position
                    const float scale = 1.f;
                    
                    //DrawModel(model, position, scale, WHITE);
                }

                Matrix M = MatrixIdentity();
                DrawTest(M);
            }
            EndMode3D();
            
            //debug textures
            if(ShowTextures)
            {
                for(int i=0; i < 3; i++)
                {
                    DrawTexture(textures[i], i * 100, 0, WHITE);
                }
            }
        }
        EndDrawing();
    }

    UnloadModel(model);

    CloseWindow();
    return 0;
}