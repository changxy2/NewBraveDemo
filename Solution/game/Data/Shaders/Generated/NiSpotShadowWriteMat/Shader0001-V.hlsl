#line 2 "d:\NewDemo\Solution\game\Data\Shaders\Generated\NiSpotShadowWriteMat\Shader0001-V.hlsl"
/*
Shader description:

*/

//---------------------------------------------------------------------------
// Types:
#if defined(DIRECT3D)
#define BONEMATRIX_TYPE float4x3
#else
#define BONEMATRIX_TYPE float3x4
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant variables:
//---------------------------------------------------------------------------

BONEMATRIX_TYPE g_SkinBoneMatrix3[30];
float4x4 g_ViewProj;
//---------------------------------------------------------------------------
// Functions:
//---------------------------------------------------------------------------

/*

    This fragment is responsible for applying the view projection and skinning 
    transform to the input position. Additionally, this fragment applies the 
    computed world transform to the input position. The weighted world 
    transform defined by the blendweights is output for use in normals or
    other calculations as the new world matrix.
    
*/

void TransformSkinnedPosition(float3 Position,
    int4 BlendIndices,
    float3 BlendWeights,
    BONEMATRIX_TYPE Bones[30],
    out float4 WorldPos,
    out float4x4 SkinBoneTransform)
{

    // TransformSkinnedPosition *********************************************
    // Transform the skinned position into world space
    // Composite the skinning transform which will take the vertex
    // and normal to world space.
    float fWeight3 = 1.0 - BlendWeights[0] - BlendWeights[1] - BlendWeights[2];
    BONEMATRIX_TYPE ShortSkinBoneTransform;
    ShortSkinBoneTransform  = Bones[BlendIndices[0]] * BlendWeights[0];
    ShortSkinBoneTransform += Bones[BlendIndices[1]] * BlendWeights[1];
    ShortSkinBoneTransform += Bones[BlendIndices[2]] * BlendWeights[2];
    ShortSkinBoneTransform += Bones[BlendIndices[3]] * fWeight3;
    SkinBoneTransform = float4x4(ShortSkinBoneTransform[0], 0.0f, 
        ShortSkinBoneTransform[1], 0.0f, 
        ShortSkinBoneTransform[2], 0.0f, 
        ShortSkinBoneTransform[3], 1.0f);

    // Transform into world space.
    WorldPos.xyz = mul(float4(Position, 1.0), ShortSkinBoneTransform);
    WorldPos.w = 1.0f;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for applying the view projection transform
    to the input world position.
    
*/

void ProjectPositionWorldToProj(float4 WorldPosition,
    float4x4 ViewProjection,
    out float4 ProjPos)
{

    ProjPos = mul(WorldPosition, ViewProjection);
    
}
//---------------------------------------------------------------------------
/*

      This fragment splits and passes through a single float4 input into two
      float4 outputs.
    
*/

void TeeFloat4(float4 Input,
    out float4 Output1,
    out float4 Output2)
{

      Output1 = Input;
      Output2 = Input;
    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Input:
//---------------------------------------------------------------------------

struct Input
{
    float3 Position : POSITION0;
    float3 BlendWeights : BLENDWEIGHT0;
    int4 BlendIndices : BLENDINDICES0;

};

//---------------------------------------------------------------------------
// Output:
//---------------------------------------------------------------------------

struct Output
{
    float4 PosProjected : POSITION0;
    float4 PosProjectedPassThrough : TEXCOORD0;

};

//---------------------------------------------------------------------------
// Main():
//---------------------------------------------------------------------------

Output Main(Input In)
{
    Output Out;
	// Function call #0
    float4 WorldPos_CallOut0;
    float4x4 SkinBoneTransform_CallOut0;
    TransformSkinnedPosition(In.Position, In.BlendIndices, In.BlendWeights, 
        g_SkinBoneMatrix3, WorldPos_CallOut0, SkinBoneTransform_CallOut0);

	// Function call #1
    float4 ProjPos_CallOut1;
    ProjectPositionWorldToProj(WorldPos_CallOut0, g_ViewProj, ProjPos_CallOut1);

	// Function call #2
    TeeFloat4(ProjPos_CallOut1, Out.PosProjectedPassThrough, Out.PosProjected);

    return Out;
}

