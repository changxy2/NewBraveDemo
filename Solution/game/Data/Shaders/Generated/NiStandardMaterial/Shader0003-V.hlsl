#line 2 "d:\NewDemo\Solution\game\Data\Shaders\Generated\NiStandardMaterial\Shader0003-V.hlsl"
/*
Shader description:
TRANSFORM = 1
OUTPUTWORLDPOS = 0
OUTPUTWORLDNBT = 0
OUTPUTWORLDVIEW = 0
OUTPUTTANGENTVIEW = 0
NORMAL = 1
SPECULAR = 0
FOGTYPE = 0
ENVMAPTYPE = 0
PROJLIGHTMAPCOUNT = 0
PROJLIGHTMAPTYPES = 0
PROJSHADOWMAPCOUNT = 0
PROJSHADOWMAPTYPES = 0
OUTPUTUVCOUNT = 1
UVSET00 = 0
UVSET00TEXOUTPUT = 0
UVSET01 = 0
UVSET01TEXOUTPUT = 0
UVSET02 = 0
UVSET02TEXOUTPUT = 0
UVSET03 = 0
UVSET03TEXOUTPUT = 0
UVSET04 = 0
UVSET04TEXOUTPUT = 0
UVSET05 = 0
UVSET05TEXOUTPUT = 0
UVSET06 = 0
UVSET06TEXOUTPUT = 0
UVSET07 = 0
UVSET07TEXOUTPUT = 0
UVSET08 = 0
UVSET08TEXOUTPUT = 0
UVSET09 = 0
UVSET09TEXOUTPUT = 0
UVSET10 = 0
UVSET10TEXOUTPUT = 0
UVSET11 = 0
UVSET11TEXOUTPUT = 0
POINTLIGHTCOUNT = 0
SPOTLIGHTCOUNT = 0
DIRLIGHTCOUNT = 1
VERTEXCOLORS = 0
VERTEXLIGHTSONLY = 1
AMBDIFFEMISSIVE = 0
LIGHTINGMODE = 1
APPLYMODE = 1
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
float4 g_MaterialEmissive;
float4 g_MaterialDiffuse;
float4 g_MaterialAmbient;
float4 g_AmbientLight;
float4 g_DirAmbient0;
float4 g_DirDiffuse0;
float4 g_DirSpecular0;
float4 g_DirWorldPosition0;
float4 g_DirWorldDirection0;
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

    This fragment is responsible for applying the world transform to the
    normal.
    
*/

void TransformNormal(float3 Normal,
    float4x4 World,
    out float3 WorldNrm)
{

    // Transform the normal into world space for lighting
    WorldNrm = mul( Normal, (float3x3)World );

    // Should not need to normalize here since we will normalize in the pixel 
    // shader due to linear interpolation across triangle not preserving
    // normality.
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for normalizing a float3.
    
*/

void NormalizeFloat3(float3 VectorIn,
    out float3 VectorOut)
{

    VectorOut = normalize(VectorIn);
    
}
//---------------------------------------------------------------------------
/*

    Separate a float4 into a float3 and a float.   
    
*/

void SplitColorAndOpacity(float4 ColorAndOpacity,
    out float3 Color,
    out float Opacity)
{

    Color.rgb = ColorAndOpacity.rgb;
    Opacity = ColorAndOpacity.a;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for accumulating the effect of a light
    on the current pixel.
    
    LightType can be one of three values:
        0 - Directional
        1 - Point 
        2 - Spot
        
    Note that the LightType must be a compile-time variable,
    not a runtime constant/uniform variable on most Shader Model 2.0 cards.
    
    The compiler will optimize out any constants that aren't used.
    
    Attenuation is defined as (const, linear, quad, range).
    Range is not implemented at this time.
    
    SpotAttenuation is stored as (cos(theta/2), cos(phi/2), falloff)
    theta is the angle of the inner cone and phi is the angle of the outer
    cone in the traditional DX manner. Gamebryo only allows setting of
    phi, so cos(theta/2) will typically be cos(0) or 1. To disable spot
    effects entirely, set cos(theta/2) and cos(phi/2) to -1 or lower.
    
*/

void Light(float4 WorldPos,
    float3 WorldNrm,
    int LightType,
    bool SpecularEnable,
    float Shadow,
    float3 WorldViewVector,
    float4 LightPos,
    float3 LightAmbient,
    float3 LightDiffuse,
    float3 LightSpecular,
    float3 LightAttenuation,
    float3 LightSpotAttenuation,
    float3 LightDirection,
    float4 SpecularPower,
    float3 AmbientAccum,
    float3 DiffuseAccum,
    float3 SpecularAccum,
    out float3 AmbientAccumOut,
    out float3 DiffuseAccumOut,
    out float3 SpecularAccumOut)
{
   
    // Get the world space light vector.
    float3 LightVector;
    float DistanceToLight;
    float DistanceToLightSquared;
        
    if (LightType == 0)
    {
        LightVector = -LightDirection;
    }
    else
    {
        LightVector = LightPos - WorldPos;
        DistanceToLightSquared = dot(LightVector, LightVector);
        DistanceToLight = length(LightVector);
        LightVector = normalize(LightVector);
    }
    
    // Take N dot L as intensity.
    float LightNDotL = dot(LightVector, WorldNrm);
    float LightIntensity = max(0, LightNDotL);

    float Attenuate = Shadow;
    
    if (LightType != 0)
    {
        // Attenuate Here
        Attenuate = LightAttenuation.x +
            LightAttenuation.y * DistanceToLight +
            LightAttenuation.z * DistanceToLightSquared;
        Attenuate = max(1.0, Attenuate);
        Attenuate = 1.0 / Attenuate;
        Attenuate *= Shadow;

        if (LightType == 2)
        {
            // Get intensity as cosine of light vector and direction.
            float CosAlpha = dot(-LightVector, LightDirection);

            // Factor in inner and outer cone angles.
            float AttenDiff = LightSpotAttenuation.x - LightSpotAttenuation.y;
            CosAlpha = saturate((CosAlpha - LightSpotAttenuation.y) / 
                AttenDiff);

            // Power to falloff.
            // The pow() here can create a NaN if CosAlpha is 0 or less.
            // On some cards (GeForce 6800), the NaN will propagate through
            // a ternary instruction, so we need two to be safe.
            float origCosAlpha = CosAlpha;
            CosAlpha = origCosAlpha <= 0.0 ? 1.0 : CosAlpha;
            CosAlpha = pow(CosAlpha, LightSpotAttenuation.z);
            CosAlpha = origCosAlpha <= 0.0 ? 0.0 : CosAlpha;

            // Multiply the spot attenuation into the overall attenuation.
            Attenuate *= CosAlpha;
        }
    }
    // Determine the interaction of diffuse color of light and material.
    // Scale by the attenuated intensity.
    DiffuseAccumOut = DiffuseAccum;
    DiffuseAccumOut.rgb += LightDiffuse.rgb * LightIntensity * Attenuate;

    // Determine ambient contribution - Is affected by shadow
    AmbientAccumOut = AmbientAccum;
    AmbientAccumOut.rgb += LightAmbient.rgb * Attenuate;

    SpecularAccumOut = SpecularAccum;
    if (SpecularEnable)
    {
        // Get the half vector.
        float3 LightHalfVector = LightVector + WorldViewVector;
        LightHalfVector = normalize(LightHalfVector);

        // Determine specular intensity.
        float LightNDotH = max(0.00001f, dot(WorldNrm, LightHalfVector));
        float LightSpecIntensity = pow(LightNDotH, SpecularPower.x);
        
        //if (LightNDotL < 0.0)
        //    LightSpecIntensity = 0.0;
        // Must use the code below rather than code above.
        // Using previous lines will cause the compiler to generate incorrect
        // output.
        float SpecularMultiplier = LightNDotL > 0.0 ? 1.0 : 0.0;
        
        // Attenuate Here
        LightSpecIntensity = LightSpecIntensity * Attenuate * 
            SpecularMultiplier;
        
        // Determine the interaction of specular color of light and material.
        // Scale by the attenuated intensity.
        SpecularAccumOut.rgb += LightSpecIntensity * LightSpecular;
    }       

    
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the coefficients for the 
    following equations:
    
    Kdiffuse = MatEmissive + 
        MatAmbient * Summation(0...N){LightAmbientContribution[N]} + 
        MatDiffuse * Summation(0..N){LightDiffuseContribution[N]}
        
    Kspecular = MatSpecular * Summation(0..N){LightSpecularContribution[N]}
    
    
*/

void ComputeShadingCoefficients(float3 MatEmissive,
    float3 MatDiffuse,
    float3 MatAmbient,
    float3 MatSpecular,
    float3 LightSpecularAccum,
    float3 LightDiffuseAccum,
    float3 LightAmbientAccum,
    bool Saturate,
    out float3 Diffuse,
    out float3 Specular)
{

    Diffuse = MatEmissive + MatAmbient * LightAmbientAccum + 
        MatDiffuse * LightDiffuseAccum;
    Specular = MatSpecular * LightSpecularAccum;
    
    if (Saturate)
    {
        Diffuse = saturate(Diffuse);
        Specular = saturate(Specular);
    }
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the final RGBA color.
    
*/

void CompositeFinalRGBAColor(float3 FinalColor,
    float FinalOpacity,
    out float4 OutputColor)
{

    OutputColor.rgb = FinalColor.rgb;
    OutputColor.a = saturate(FinalOpacity);
    
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
    float3 Normal : NORMAL0;
    float2 UVSet0 : TEXCOORD0;

};

//---------------------------------------------------------------------------
// Output:
//---------------------------------------------------------------------------

struct Output
{
    float4 PosProjected : POSITION0;
    float4 DiffuseAccum : TEXCOORD0;
    float2 UVSet0 : TEXCOORD1;

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
    ProjectPositionWorldToProj(WorldPos_CallOut0, g_ViewProj, Out.PosProjected);

	// Function call #2
    float3 WorldNrm_CallOut2;
    TransformNormal(In.Normal, SkinBoneTransform_CallOut0, WorldNrm_CallOut2);

	// Function call #3
    float3 VectorOut_CallOut3;
    NormalizeFloat3(WorldNrm_CallOut2, VectorOut_CallOut3);

	// Function call #4
    float3 Color_CallOut4;
    float Opacity_CallOut4;
    SplitColorAndOpacity(g_MaterialDiffuse, Color_CallOut4, Opacity_CallOut4);

	// Function call #5
    float3 AmbientAccumOut_CallOut5;
    float3 DiffuseAccumOut_CallOut5;
    float3 SpecularAccumOut_CallOut5;
    Light(WorldPos_CallOut0, VectorOut_CallOut3, int(0), bool(false), 
        float(1.0), float3(0.0, 0.0, 0.0), g_DirWorldPosition0, g_DirAmbient0, 
        g_DirDiffuse0, g_DirSpecular0, float3(0.0, 1.0, 0.0), 
        float3(-1.0, -1.0, 0.0), g_DirWorldDirection0, 
        float4(1.0, 1.0, 1.0, 1.0), g_AmbientLight, float3(0.0, 0.0, 0.0), 
        float3(0.0, 0.0, 0.0), AmbientAccumOut_CallOut5, 
        DiffuseAccumOut_CallOut5, SpecularAccumOut_CallOut5);

	// Function call #6
    float3 Diffuse_CallOut6;
    float3 Specular_CallOut6;
    ComputeShadingCoefficients(g_MaterialEmissive, Color_CallOut4, 
        g_MaterialAmbient, float3(1.0, 1.0, 1.0), float3(0.0, 0.0, 0.0), 
        DiffuseAccumOut_CallOut5, AmbientAccumOut_CallOut5, bool(false), 
        Diffuse_CallOut6, Specular_CallOut6);

	// Function call #7
    CompositeFinalRGBAColor(Diffuse_CallOut6, Opacity_CallOut4, 
        Out.DiffuseAccum);

    Out.UVSet0 = In.UVSet0;
    return Out;
}

