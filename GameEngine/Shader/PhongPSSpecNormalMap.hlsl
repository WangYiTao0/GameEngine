#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonCbuf.hlsli"
#include "ShadingMath.hlsli"

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float4 lightSpacePos : POSITION1;
    float3 worldNormal : Normal;
    float3 worldTan : Tangent;
    float3 worldBitan : Bitangent;
    float2 texcoord : Texcoord;
};

//cbuffer ObjectCBuf : register(b2)
//{
//    float3 diff;
//    float shininess;
//    float3 spec;
//    float padding;
//};

cbuffer ObjectCBuf : register(b2)
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D diffTex : register(t0);
Texture2D specTex : register(t1);
Texture2D nmapTex : register(t2);
Texture2D shadowTex : register(t3);


SamplerState sampleAn : register(s0);
SamplerState sampleClamp : register(s1);
float4 main(PS_pIn pIn) : SV_Target
{
    //alpha blender
    float4 texDiff = diffTex.Sample(sampleAn, pIn.texcoord).rgba;
    
    // fragment to light vector data
    #ifdef HASMASK
    // bail if highly translucent
    clip(texDiff.a < 0.1f ? -1 : 1);

    float3 toEye = cameraPos-pIn.worldPos;
    float3 dirToL = gLights[0].position-pIn.worldPos;

    // flip normal when backface
    if (dot(pIn.worldNormal, dirToL) >= 0.0f && dot(pIn.worldNormal, -toEye) >= 0.0f)
    {
        pIn.worldNormal = -pIn.worldNormal;
    }
    #endif

    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    pIn.worldTan = normalize(pIn.worldTan);
    pIn.worldBitan = normalize(pIn.worldBitan);
    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
    //pIn.worldNormal = UnpackNormals(nmap, sample0, pIn.texcoord, pIn.worldNormal, pIn.worldTan);
        pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, nmapTex, sampleAn);
    }
    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float shadowFactor[MaxLights];
    for (int i = 0; i < MaxLights; i++)
    {
        shadowFactor[i] = 1.0f;
    }

   // shadowFactor[0] = 1.0f - ShadowCalculation(pIn.lightSpacePos, shadowTex, sampleClamp, gLights[0].position, pIn.worldPos, pIn.worldNormal);


    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = specTex.Sample(sampleAn, pIn.texcoord);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }

    //float3 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgb;
    float3 texSpec = specTex.Sample(sampleAn, pIn.texcoord).rgb;

    Material mat = { texDiff.rgb, specularReflectionColor, specularPower };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);
    finalColor.a = texDiff.a;
    // final color
    return finalColor;
}
