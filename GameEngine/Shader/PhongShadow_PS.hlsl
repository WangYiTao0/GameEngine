/*Pointlight 
1.vector to light pos  v_to_l = light_pos - pos
2 distance = v_to_light.len();
3 direction = v_to_light /dist;

light Pos
4 Canculate attenuation
  Attenuation = 1 /(Ld*d+Qd+C)
  linear attenuation
  quadradic attnuation
  constant attnuation
5 calculate intensity based on angle of incidence and attenuation
6 add diffuse+ambient, filter by material color, saturate and scale
*/ 


#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonCbuf.hlsli"



//cbuffer ObjectCBuf : register(b2)
//{
//    float3 diff;
//    float shininess;
//    float3 spec;
//    float padding;
//};


cbuffer ObjectCBuf : register(b2)
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

struct PS_pIn
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float4 lightSpacePos : POSITION1;
    float3 worldNormal : Normal;
    float2 texcoord : Texcoord;
};

Texture2D diffTex : register(t0);
Texture2D nmapTex : register(t2);
Texture2D shadowTex : register(t3);

SamplerState sampleWarp : register(s0);
SamplerState sampleClamp : register(s1);

float4 main(PS_pIn pIn) : SV_Target
{

    //renormalize interpolatednormal
    pIn.worldNormal = normalize(pIn.worldNormal);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

    float shadowFactor[MaxLights];
    for (int i = 0; i < MaxLights; i++)
    {
        shadowFactor[i] = 1.0f;
    }
    shadowFactor[0] = 1.0f - ShadowCalculation(pIn.lightSpacePos, shadowTex, sampleClamp, gLights[0].position, pIn.worldPos, pIn.worldNormal);

    float3 texDiff = diffTex.Sample(sampleWarp, pIn.texcoord).rgb;

    Material mat = { texDiff, specularIntensity.rrr, specularPower };
    float4 finalColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    // final color
    return finalColor;
}