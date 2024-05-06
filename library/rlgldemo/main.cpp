/*
    libvitaboy - Open source Openrl TSO character animation library
    Renderer.cpp - Copyright (c) 2012 Niotso Project <http://niotso.org/>
    Author(s): Fatbag <X-Fi6@phppoll.org>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NErlIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "raylib.h"
#include "raymath.h"        // Required for: Vector3, Quaternion and Matrix functionality
#include "utils.h"          // Required for: TRACELOG(), LoadFileData(), LoadFileText(), SaveFileText()
#include "rlgl.h"

#include <cassert>
#include <FileHandler.h>
#include "../libvitaboy/libvitaboy.hpp"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#define WINDOW_TITLE "libvitaboy - Renderer - Ray"


struct BasicVertex_t {
    float x, y, z;
};
struct CharacterPlacement_t {
    BasicVertex_t Translation;
    BasicVertex_t Rotation;
};
static CharacterPlacement_t Character = {{0,-3,0}, {0,0,0}};

static Skeleton_t Skeleton;

static Animation_t Animation;
static float AnimationTime = 0;

static void DisplayFileError(const char * Filename){
    const char * Message;
    switch(File::Error){
    case FERR_NOT_FOUND:
        Message = "%s does not exist.";
        break;
    case FERR_OPEN:
        Message = "%s could not be opened for reading.";
        break;
    case FERR_BLANK:
    case FERR_UNRECOGNIZED:
    case FERR_INVALIDDATA:
        Message = "%s is corrupt or invalid.";
        break;
    case FERR_MEMORY:
        Message = "Memory for %s could not be allocated.";
        break;
    default:
        Message = "%s could not be read.";
        break;
    }

    char Buffer[1024];
    sprintf(Buffer, Message, Filename);
}

static bool Read(const char* Filename, uint8_t** InData) {
    *InData = File::ReadFile(Filename);
    if (*InData != NULL) {
        VBFile.set(*InData, File::FileSize);
        return true;
    }

    DisplayFileError(Filename);
    return false;
}

//textures
static Texture2D textures[3];
enum { Texture_Body, Texture_Head, Texture_Hand };
static const char* const TexturePaths[] = { "body.jpg", "head.jpg", "hand.jpg" };

static bool LoadTextures()
{
    for (int i = 0; i < 3; i++)
    {
        textures[i] = LoadTexture(TexturePaths[i]);
    }
    return true;
}

static bool UnloadTextures()
{
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(textures[i]);
    }
    return true;
}

//meshes
static const unsigned MeshCount = 4;
static Mesh_t Meshes[4];
enum { Mesh_Body, Mesh_Head, Mesh_LHand, Mesh_RHand };
static const char* const MeshPaths[] = { "body.mesh", "head.mesh", "lhand.mesh", "rhand.mesh" };
static const unsigned Mesh_UseTexture[] = { Texture_Body, Texture_Head, Texture_Hand, Texture_Hand };
static const char* const MeshActivate[] = { NULL, "HEAD", "L_HAND", "R_HAND" };

// Generate a simple triangle mesh from code
static Mesh CreateMesh(const Mesh_t& tso_mesh)
{
    Mesh ray_mesh = { 0 };
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

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&ray_mesh, true); //check the dynamic flag if we have to keep creating the mesh
    return ray_mesh;
}

static bool LoadMeshes()
{
    uint8_t* InData;
    for (unsigned i = 0; i < MeshCount; i++)
    {
        if (!Read(MeshPaths[i], &InData))
        {
            return false;
        }
        ReadMesh(Meshes[i]);
        free(InData);
    }
    return true;
}

static int ResizeScene(uint16_t width, uint16_t height)
{
    rlViewport(0, 0, width, height);

    rlMatrixMode(RL_PROJECTION);
    rlLoadIdentity();

    // Calculate The Aspect Ratio Of The Window
    //rluPerspective(45.0f, (rlfloat)width/(rlfloat)height, 0.1f, 100.0f);
    // rlScalef(-1.0f, 1.0f, 1.0f);

    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();
    return 1;
}

static bool InitRLGL()
{
    rlglInit(SCREEN_WIDTH, SCREEN_HEIGHT);

    //rlSetCullFace();
    //rlEnable(rl_CULL_FACE);
    rlDisableBackfaceCulling();
    //rlDisable(rl_BLEND);
    rlEnableColorBlend();
    //rlDepthFunc(rl_LEQUAL); //default
    //rlHint(rl_PERSPECTIVE_CORRECTION_HINT, rl_NICEST); //default
    
    //cant change this
    //rlFrontFace(rl_CW);
    ResizeScene(SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}

static void TransformVertices(Bone_t& Bone)
{
    rlTranslatef(Bone.Translation.x, Bone.Translation.y, Bone.Translation.z);
    float matRotation[16];
    FindQuaternionMatrix(matRotation, &Bone.Rotation);
    rlMultMatrixf(matRotation);

    unsigned MeshIndex = 0;
    unsigned BoneIndex;

    for(unsigned i=1; i<MeshCount; i++){
        if(!strcmp(Bone.Name, MeshActivate[i])){
            MeshIndex = i;
            break;
        }
    }
    Mesh_t& Mesh = Meshes[MeshIndex];
    for(BoneIndex=0; BoneIndex<Mesh.BindingCount; BoneIndex++){
        if(!strcmp(Bone.Name, Mesh.BoneNames[Mesh.BoneBindings[BoneIndex].BoneIndex]))
            break;
    }

    if(BoneIndex < Mesh.BindingCount){
        for(unsigned i=0; i<Mesh.BoneBindings[BoneIndex].RealVertexCount; i++){
            unsigned VertexIndex = Mesh.BoneBindings[BoneIndex].FirstRealVertex + i;
            Vertex_t& RelativeVertex = Mesh.VertexData[VertexIndex];
            Vertex_t& AbsoluteVertex = Mesh.TransformedVertexData[VertexIndex];

            
            rlTranslatef(RelativeVertex.Coord.x, RelativeVertex.Coord.y, RelativeVertex.Coord.z);
            const Matrix matModelView = rlGetMatrixModelview();
            const Vector3 vertex = Vector3Transform(Vector3{ 0.f, 0.f, 0.f }, matModelView);
            AbsoluteVertex.Coord.x = vertex.x;
            AbsoluteVertex.Coord.y = vertex.y;
            AbsoluteVertex.Coord.z = vertex.z;
            rlTranslatef(-RelativeVertex.Coord.x, -RelativeVertex.Coord.y, -RelativeVertex.Coord.z);
        }
        for(unsigned i=0; i<Mesh.BoneBindings[BoneIndex].BlendVertexCount; i++){
            unsigned VertexIndex = Mesh.RealVertexCount + Mesh.BoneBindings[BoneIndex].FirstBlendVertex + i;
            Vertex_t& RelativeVertex = Mesh.VertexData[VertexIndex];
            Vertex_t& AbsoluteVertex = Mesh.TransformedVertexData[VertexIndex];

            rlTranslatef(RelativeVertex.Coord.x, RelativeVertex.Coord.y, RelativeVertex.Coord.z);
            const Matrix matModelView = rlGetMatrixModelview();
            const Vector3 vertex = Vector3Transform(Vector3{ 0.f, 0.f, 0.f }, matModelView);
            AbsoluteVertex.Coord.x = vertex.x;
            AbsoluteVertex.Coord.y = vertex.y;
            AbsoluteVertex.Coord.z = vertex.z;
            rlTranslatef(-RelativeVertex.Coord.x, -RelativeVertex.Coord.y, -RelativeVertex.Coord.z);
        }
    }

    if(Bone.ChildrenCount == 1){
        TransformVertices(*Bone.Children[0]);
    }else if(Bone.ChildrenCount > 1){
        for(unsigned i=0; i<Bone.ChildrenCount; i++){
            rlPushMatrix();
            TransformVertices(*Bone.Children[i]);
            rlPopMatrix();
        }
    }
}

static void BlendVertices()
{
    for(unsigned i=0; i<MeshCount; i++){
        Mesh_t& Mesh = Meshes[i];
        for(unsigned i=0; i<Mesh.BlendVertexCount; i++){
            Vertex_t& BlendVertex = Mesh.TransformedVertexData[Mesh.RealVertexCount + i];
            float Weight = BlendVertex.BlendData.Weight;
            Vertex_t& RealVertex = Mesh.TransformedVertexData[BlendVertex.BlendData.OtherVertex];
            RealVertex.Coord.x =
                Weight     * BlendVertex.Coord.x +
                (1-Weight) * RealVertex.Coord.x;
            RealVertex.Coord.y =
                Weight     * BlendVertex.Coord.y +
                (1-Weight) * RealVertex.Coord.y;
            RealVertex.Coord.z =
                Weight     * BlendVertex.Coord.z +
                (1-Weight) * RealVertex.Coord.z;
        }
    }
}

static Mesh ray_meshes[4];
static void CreateMeshes()
{
    for (unsigned i = 0; i < MeshCount; i++)
    {
        ray_meshes[i] = CreateMesh(Meshes[i]);
    }
}

static void DrawMeshes()
{
    rlPushMatrix();
    rlLoadIdentity();
    TransformVertices(Skeleton.Bones[0]);
    rlPopMatrix();
    BlendVertices();

    Material material = LoadMaterialDefault();
    for (unsigned i = 0; i < MeshCount; i++)
    {
        material.maps[MATERIAL_MAP_DIFFUSE].texture = textures[Mesh_UseTexture[i]];
        DrawMesh(ray_meshes[i], material, MatrixIdentity());
    }
    rlPopMatrix();
}

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

static void DrawBonesSkeleton(Bone_t& Bone)
{
    rlTranslatef(Bone.Translation.x, Bone.Translation.y, Bone.Translation.z);
    float RotationMatrix[16];
    FindQuaternionMatrix(RotationMatrix, &Bone.Rotation);
    rlMultMatrixf(RotationMatrix);

    Color color;
    if (!strcmp(Bone.Name, "ROOT"))
    {
        color = RED;
    }
    else if (!strcmp(Bone.Name, "HEAD"))
    {
        color = YELLOW;
    }
    else
    {
        color = GREEN;
    }
    DrawCube(Vector3{ 0.f, 0.f, 0.f }, 0.1f, 0.1f, 0.1f, color);

    if(Bone.ChildrenCount == 1){
        DrawBonesSkeleton(*Bone.Children[0]);
    }else if(Bone.ChildrenCount > 1){
        for(unsigned i=0; i<Bone.ChildrenCount; i++){
            rlPushMatrix();
            {
                DrawBonesSkeleton(*Bone.Children[i]);
            }
            rlPopMatrix();
        }
    }
}

static void DrawSkeleton()
{
    rlPushMatrix();
    {
        DrawBonesSkeleton(Skeleton.Bones[0]);
    }
    rlPopMatrix();
}

static bool Startup()
{
    uint8_t * InData;
    if(!Read("skeleton.skel", &InData))
        return false;
    ReadSkeleton(Skeleton);
    free(InData);

    for(unsigned i=0; i<MeshCount; i++){
        if(!Read(MeshPaths[i], &InData))
            return 0;
        ReadMesh(Meshes[i]);
        free(InData);
    }

    if(!Read("animation.anim", &InData))
        return false;
    ReadAnimation(Animation);
    free(InData);

    AdvanceFrame(Skeleton, Animation, 0);
    return true;
}

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
        Mesh& ray_mesh = model.meshes[i];

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
            //ray_mesh.vertices[j * 3 + 0] = tso_mesh.TransformedVertexData[j].Coord.x;
            //ray_mesh.vertices[j * 3 + 1] = tso_mesh.TransformedVertexData[j].Coord.y;
            //ray_mesh.vertices[j * 3 + 2] = tso_mesh.TransformedVertexData[j].Coord.z;
            ray_mesh.vertices[j * 3 + 0] = tso_mesh.VertexData[j].Coord.x;
            ray_mesh.vertices[j * 3 + 1] = tso_mesh.VertexData[j].Coord.y;
            ray_mesh.vertices[j * 3 + 2] = tso_mesh.VertexData[j].Coord.z; //offset for now so we can see them
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
            //ray_mesh.indices[j * 3 + 1] = (unsigned short)tso_mesh.FaceData[j].VertexC;
            //ray_mesh.indices[j * 3 + 2] = (unsigned short)tso_mesh.FaceData[j].VertexB;
            //Clock wise
            ray_mesh.indices[j * 3 + 1] = (unsigned short)tso_mesh.FaceData[j].VertexB;
            ray_mesh.indices[j * 3 + 2] = (unsigned short)tso_mesh.FaceData[j].VertexC;
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
        for (int i = 0; i < model.meshCount; i++)
        {
            UploadMesh(&model.meshes[i], true);
        }
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

//settings
static bool ShowMesh = true;
static bool ShowSkeleton = true;
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    InitRLGL();

    // Define the camera to look into our 3d world
    Camera camera;
    camera.position = Vector3{ 0.0f, 5.0f, 5.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera projection type

    assert( Startup() );
    assert( LoadTextures() );
    assert( LoadMeshes() );
    CreateMeshes();

    TransformVertices(Skeleton.Bones[0]);
    BlendVertices();
    Model model = LoadModelTSO();

    printf("======================================\n");
    printf("=================RAY==================\n");
    printf("======================================\n");

    DisableCursor();

    while (!WindowShouldClose())
    {
        const float dt{ GetFrameTime() };
        //Handle user interaction
        {
            UpdateCamera(&camera, CAMERA_FREE);

            //if (IsKeyDown(KEY_A))      /*{if(zoom <=-1.0f)  zoom+=0.05f; }*/ zoom += 3 * dt;
            //if (IsKeyDown(KEY_S))      /*{if(zoom >=-10.0f) zoom-=0.05f; }*/ zoom -= 3 * dt;
            if (IsKeyDown(KEY_UP)) { if ((Character.Rotation.x -= 60 * dt) <= -360) Character.Rotation.x += 360; }
            if (IsKeyDown(KEY_DOWN)) { if ((Character.Rotation.x += 60 * dt) >= 360)  Character.Rotation.x -= 360; }
            if (IsKeyDown(KEY_LEFT)) { if ((Character.Rotation.y -= 60 * dt) <= -360) Character.Rotation.y += 360; }
            if (IsKeyDown(KEY_RIGHT)) { if ((Character.Rotation.y += 60 * dt) >= 360)  Character.Rotation.y -= 360; }
            if (IsKeyDown(KEY_X)) { if ((Character.Rotation.z -= 60 * dt) <= -360) Character.Rotation.z += 360; }
            if (IsKeyDown(KEY_Z)) { if ((Character.Rotation.z += 60 * dt) >= 360)  Character.Rotation.z -= 360; }
            if (IsKeyDown(KEY_K)) { Character.Translation.y -= 3 * dt; }
            if (IsKeyDown(KEY_I)) { Character.Translation.y += 3 * dt; }
            if (IsKeyDown(KEY_J)) { Character.Translation.x -= 3 * dt; }
            if (IsKeyDown(KEY_L)) { Character.Translation.x += 3 * dt; }
            if (IsKeyPressed(KEY_ONE)){ ShowSkeleton = !ShowSkeleton; }
            if (IsKeyPressed(KEY_TWO)) { ShowMesh = !ShowMesh; }
            if (IsKeyDown(KEY_N)) { AdvanceFrame(Skeleton, Animation, dt); }
        }

        //rlClearScreenBuffers(); //Clear the screen and the depth buffer

        BeginDrawing();
        {
            ClearBackground(BLACK);
            BeginMode3D(camera);
            {
                DrawGrid(10, 5.0f);

                //Avatar
                {
                    if (ShowMesh)
                    {
                        //rlEnableWireMode();
                        //rlColor3f(1.0, 1.0, 1.0);
                        //DrawMeshes();
                        //rlDisableWireMode();
                        const Vector3 position{ 0.0f, 0.0f, 0.0f };
                        const float scale = 1.f;
                        DrawModel(model, position, scale, WHITE);
                    }

                    if (ShowSkeleton)
                    {
                        DrawSkeleton();
                    }
                }
            }
            EndMode3D();
            
        }
        EndDrawing();
    }

    UnloadTextures();
    CloseWindow();
    return 0;
}