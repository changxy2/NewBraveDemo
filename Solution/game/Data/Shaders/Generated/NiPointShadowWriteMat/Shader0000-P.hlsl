#line 2 "d:\NewDemo\Solution\game\Data\Shaders\Generated\NiPointShadowWriteMat\Shader0000-P.hlsl"
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

//---------------------------------------------------------------------------
// Functions:
//---------------------------------------------------------------------------

/*

    This fragment writes projected depth to all color component outputs.
    
*/

void WriteDepthToColor(float3 WorldViewVector,
    out float4 OutputColor)
{

    float Depth = dot(WorldViewVector, WorldViewVector);
    OutputColor.x = sqrt(Depth);
    OutputColor.yzw = 1.0f;
    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Input:
//---------------------------------------------------------------------------

struct Input
{
    float4 PosProjected : POSITION0;
    float3 WorldViewVector : TEXCOORD0;

};

//---------------------------------------------------------------------------
// Output:
//---------------------------------------------------------------------------

struct Output
{
    float4 Color : COLOR0;

};

//---------------------------------------------------------------------------
// Main():
//---------------------------------------------------------------------------

Output Main(Input In)
{
    Output Out;
	// Function call #0
    WriteDepthToColor(In.WorldViewVector, Out.Color);

    return Out;
}

