#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"
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
    //bool hasGloss;
    //float specularPowerConst;
    //float3 specularColor;
    //float specularMapWeight;
    float4 gDiffuseAlbedo;
    float3 gFresnelR0;
    float gRoughness;
};

Texture2D diffTex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);



float4 main(PS_pIn pIn) : SV_Target
{
    
    //alpha blender
    //float4 texDiffColor = diffTex.Sample(sample0, pIn.texcoord);

    //// fragment to light vector data
    //const LightVectorData lv = CalculateLightVectorData(pL.worldPos, pIn.worldPos);
    //#ifdef HASMASK
    //// bail if highly translucent
    //clip(texDiffColor.a < 0.1f ? -1 : 1);

    //float3 toEye = pIn.worldPos - cameraPos;

    //// flip normal when backface
    //if (dot(pIn.worldNormal, lv.dirToL) >= 0.0f && dot(pIn.worldNormal, toEye) >= 0.0f)
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
    pIn.worldNormal = MapNormal(pIn.worldTan, pIn.worldBitan, pIn.worldNormal, pIn.texcoord, nmap, sample0);

    //// specular parameter determination (mapped or uniform)
    //float3 specularReflectionColor;
    ////if doesn't has specularMap  using specularPowerConst
    //float specularPower = specularPowerConst;
 
    //const float4 specularSample = spec.Sample(sample0, pIn.texcoord);
    //specularReflectionColor = specularSample.rgb * specularMapWeight;

    //specularSample.a is [0,1] need  be power scale
    //specularPower = pow(2.0, 13.0 * shininessMap)
    //if (hasGloss)
    //{
    //    specularPower = pow(2.0f, specularSample.a * 13.0f);
    //}
 
    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(cameraPos - pIn.worldPos);

  	// Indirect lighting.
    float4 ambient = gAmbientLight * gDiffuseAlbedo;


    float4 finalColor = 1.0f;
    float4 diffTexColor = diffTex.Sample(sample0, pIn.texcoord);

    const float shininess = 1.0f - gRoughness;
    Material mat = { gDiffuseAlbedo, gFresnelR0, shininess };
    float3 shadowFactor = 1.0f;
    float4 LightColor = ComputeLighting(gLights, mat, pIn.worldPos,
        pIn.worldNormal, toEyeW, shadowFactor);

    finalColor = diffTexColor * (ambient + LightColor);

    // Common convention to take alpha from diffuse material.
    finalColor.a = gDiffuseAlbedo.a;
    // final color
    return finalColor;
}
