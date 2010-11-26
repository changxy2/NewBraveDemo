#line 2 "d:\NewDemo\Solution\game\Data\Shaders\Generated\NiDirShadowWriteMat\Shader0000-P.hlsl"
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

void WriteDepthToColor(float4 WorldPosProjected,
    out float4 OutputColor)
{

    float Depth = WorldPosProjected.z / WorldPosProjected.w;
    OutputColor.x = Depth;
    OutputColor.yzw = 1.0f;
    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Input:
//---------------------------------------------------------------------------

struct Input
{
    float4 PosProjected : POSITION0;
    float4 WorldPosProjected : TEXCOORD0;

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
    WriteDepthToColor(In.WorldPosProjected, Out.Color);

    return Out;
}

