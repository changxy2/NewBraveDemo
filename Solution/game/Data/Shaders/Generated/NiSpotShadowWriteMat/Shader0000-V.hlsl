#line 2 "d:\NewDemo\Solution\game\Data\Shaders\Generated\NiSpotShadowWriteMat\Shader0000-V.hlsl"
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

float4x4 g_World;
float4x4 g_ViewProj;
//---------------------------------------------------------------------------
// Functions:
//---------------------------------------------------------------------------

/*

    This fragment is responsible for applying the view projection transform
    to the input position. Additionally, this fragment applies the world 
    transform to the input position. 
    
*/

void TransformPosition(float3 Position,
    float4x4 World,
    out float4 WorldPos)
{

    // Transform the position into world space for lighting, and projected 
    // space for display
    WorldPos = mul( float4(Position, 1.0f), World );
    
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
    TransformPosition(In.Position, g_World, WorldPos_CallOut0);

	// Function call #1
    float4 ProjPos_CallOut1;
    ProjectPositionWorldToProj(WorldPos_CallOut0, g_ViewProj, ProjPos_CallOut1);

	// Function call #2
    TeeFloat4(ProjPos_CallOut1, Out.PosProjectedPassThrough, Out.PosProjected);

    return Out;
}

