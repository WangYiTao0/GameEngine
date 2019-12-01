#include "ShaderOptions.hlsli"
#include "LightVectorData.hlsli"
#include "CommonPSOption.hlsli"


struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
    float3 viewTan : Tangent;
    float3 viewBitan : Bitangent;
    float2 texcoord : Texcoord;
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

    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
        input.viewNormal = MapNormal(input.viewTan, input.viewBitan, input.viewNormal, input.texcoord, nmap, sample0);
    }
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
    // specular
    const float3 specular = Speculate(
        diffuseColor, diffuseIntensity, input.viewNormal,
        lv.vToL, input.viewPixelPos, att, specularPower
    );
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(sample0, input.texcoord);

    // //clip(texColor.a - 0.1f);
    //clip(texColor.a < 0.1f ? -1 : 1);

    //// flip Normal when backface
    //if (dot(input.viewNormal, input.viewPixelPos) >= 0.0f)
    //{
    //    input.viewNormal = -input.viewNormal;
    //}

    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specular;
    //  finalColor.a = texColor.a;
    finalColor.a = 1.0f;
    // final color
    return finalColor;
}