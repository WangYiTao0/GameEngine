#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"
#include "ShadingMath.hlsli"


struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal   : Normal;
    float3 worldTan      : Tangent;
    float3 worldBitan    : Bitangent;
    float2 texcoord      : Texcoord;
};

cbuffer ObjectCBuf :register (b2)
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

Texture2D tex : register(t0);
Texture2D nmap : register(t2);




float4 main(PS_INPUT input) : SV_Target
{
    // normalize the mesh normal

    input.worldNormal = normalize(input.worldNormal);
    input.worldTan = normalize(input.worldTan);
    input.worldBitan = normalize(input.worldBitan);

  
    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
        //input.worldNormal = UnpackNormals(nmap, sample0, input.texcoord, input.worldNormal, input.worldTan);

       input.worldNormal = MapNormal(input.worldTan, input.worldBitan, input.worldNormal, input.texcoord, nmap, sample0);
    }
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(worldLightPos, input.worldPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diff = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.worldNormal);
    // specular
    const float3 specular = Speculate(
        diffuseColor, specularIntensity, input.worldNormal,
        lv.vToL, input.worldPos,cameraPos, att, specularPower
    );
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(sample0, input.texcoord);


    finalColor.rgb = texColor.rgb * saturate(ambient + diff) + specular;
    //  finalColor.a = texColor.a;
    finalColor.a = 1.0f;
    // final color
    return finalColor;
}