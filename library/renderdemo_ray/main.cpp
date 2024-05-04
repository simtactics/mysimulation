#include "raylib.h"
#include "raymath.h"        // Required for: Vector3, Quaternion and Matrix functionality
#include "utils.h"          // Required for: TRACELOG(), LoadFileData(), LoadFileText(), SaveFileText()

#include <string>
#include <cassert>
#include <FileHandler.h>
#include "../libvitaboy/libvitaboy.hpp"


#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)
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
static void DrawBonesSkeleton(Bone_t& Bone)
{
    const float size = 0.1f;
    const Vector3 position{Bone.Translation.x, Bone.Translation.y, Bone.Translation.z};
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
    DrawCube(position, size, size, size, color);

    if(Bone.ChildrenCount == 1)
    {
        DrawBonesSkeleton(*Bone.Children[0]);
    }
    else if(Bone.ChildrenCount > 1)
    {
        for(unsigned i=0; i<Bone.ChildrenCount; i++)
        {
            DrawBonesSkeleton(*Bone.Children[i]);
        }
    }
}

//animation
static Animation_t Animation;
static float AnimationTime = 0;

static void AdvanceFrame(Skeleton_t& Skeleton, Animation_t& Animation, float TimeDelta)
{
    float Duration = (float)Animation.Motions[0].FrameCount/30;
    AnimationTime += TimeDelta;
    AnimationTime = fmodf(AnimationTime, Duration); //Loop the animation

    for(unsigned i=0; i<Animation.MotionsCount; i++){
        unsigned BoneIndex = FindBone(Skeleton, Animation.Motions[i].BoneName, Skeleton.BoneCount);
        if(BoneIndex == (unsigned)-1) continue;

        Bone_t& Bone = Skeleton.Bones[BoneIndex];

        unsigned Frame = AnimationTime*30;
        float FractionShown = AnimationTime*30 - Frame;
        unsigned NextFrame = (Frame+1 != Animation.Motions[0].FrameCount) ? Frame+1 : 0;

        if(Animation.Motions[i].HasTranslation){
            Translation_t& Translation = Animation.Motions[i].Translations[Frame];
            Translation_t& NextTranslation = Animation.Motions[i].Translations[NextFrame];
            Bone.Translation.x = (1-FractionShown)*Translation.x + FractionShown*NextTranslation.x;
            Bone.Translation.y = (1-FractionShown)*Translation.y + FractionShown*NextTranslation.y;
            Bone.Translation.z = (1-FractionShown)*Translation.z + FractionShown*NextTranslation.z;
        }
        if(Animation.Motions[i].HasRotation){
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
            //indices
            for (unsigned j = 0; j < ray_mesh.triangleCount; j++)
            {
                ray_mesh.indices[j * 3 + 0] = (unsigned short)tso_mesh.FaceData[j].VertexA;
                ray_mesh.indices[j * 3 + 1] = (unsigned short)tso_mesh.FaceData[j].VertexC;
                ray_mesh.indices[j * 3 + 2] = (unsigned short)tso_mesh.FaceData[j].VertexB;
            }
            //vertex data
            for (unsigned j = 0; j < ray_mesh.vertexCount; j++)
            {
                //vertices?
                ray_mesh.vertices[j * 3 + 0] = tso_mesh.VertexData[j].Coord.x + (i * 2.f);
                ray_mesh.vertices[j * 3 + 1] = tso_mesh.VertexData[j].Coord.y;
                ray_mesh.vertices[j * 3 + 2] = tso_mesh.VertexData[j].Coord.z;
                //coords
                ray_mesh.texcoords[j * 2 + 0] = tso_mesh.TransformedVertexData[j].TextureCoord.u;
                ray_mesh.texcoords[j * 2 + 1] = -tso_mesh.TransformedVertexData[j].TextureCoord.v;
                //normals
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.x;
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.y;
                ray_mesh.normals[j * 3 + 0] = tso_mesh.TransformedVertexData[j].NormalCoord.z;
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

        return result;
    }

    //https://gist.github.com/Gamerfiend/18206474679bf5873925c839d0d6a6d0
    void LoadSkeletonTSO()
    {
        //Raylib combines skeleton data with the model data
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
    camera.position = Vector3{ 0.f, 3.0f, 5.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };  // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };      // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                          // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;       // Camera projection type - we will probably need orthographic

    //doesn't work when ran from VSStudio
    assert( Startup() );
    assert( LoadTextures() );
    assert( LoadMeshes() );

    Model model = CustomRay::LoadModelTSO();

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
        }

        BeginDrawing();
        {
            ClearBackground(DARKPURPLE);
            BeginMode3D(camera);
            {
                DrawGrid(10, 5.0f);
                if(ShowSkeleton)
                {
                    DrawBonesSkeleton(Skeleton.Bones[0]);
                }

                if (ShowMesh)
                {
                    const Vector3 position{ 0.0f, 0.0f, 0.0f }; // Set model position
                    const float scale = 1.f;
                    DrawModel(model, position, scale, WHITE);
                }
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

    CloseWindow();
    return 0;
}