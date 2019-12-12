#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonCbuf.hlsli"
#include "ShadingMath.hlsli"

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float3 worldTan : Tangent;
    float3 worldBitan : Bitangent;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf : register(b2)
{
    float3 diff;
    float shininess;
    float3 spec;
    float padding;
};

Texture2D diffTex : register(t0);
Texture2D specTex : register(t1);
Texture2D nmapTex : register(t2);


SamplerState sample0 : register(s0);

float4 main(PS_pIn pIn) : SV_Target
{
    //alpha blender
    float4 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgba;
    clip(texDiff.a < 0.1f ? -1 : 1);
    
    //// fragment to light vector data
    //#ifdef HASMASK
    //// bail if highly translucent
    //clip(texDiff.a < 0.1f ? -1 : 1);

    //float3 toEye = cameraPos-pIn.worldPos;

    //// flip normal when backface
    //if (dot(pIn.worldNormal, lv.dirToL) >= 0.0f && dot(pIn.worldNormal, -toEye) >= 0.0f)
    //{
    //    pIn.worldNormal = -pIn.worldNormal;
    //}
    //#endif

    // normalize the mesh normal
    pIn.worldNormal = normalize(pIn.worldNormal);
    pIn.worldTan = normalize(pIn.worldTan);
    pIn.worldBitan = normalize(pIn.worldBitan);
    // replace normal with mapped if normal mapping enabled

    //pIn.worldNormal = UnpackNormals(nmap, sample0, pIn.texcoord, pIn.worldNormal, pIn.worldTan);
    pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, nmapTex, sample0);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float3 shadowFactor = 1.0f;

    //float3 texDiff = diffTex.Sample(sample0, pIn.texcoord).rgb;
    float3 texSpec = specTex.Sample(sample0, pIn.texcoord).rgb;
    Material mat = { texDiff.rgb, shininess, texSpec };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);
    finalColor.a = texDiff.a;
    // final color
    return finalColor;
}
