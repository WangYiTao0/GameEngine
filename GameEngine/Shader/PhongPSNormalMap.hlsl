#include "ShaderOptions.hlsli"
#include "LightVectorData.hlsli"

#include "LightOptions.hlsli"

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

Texture2D tex;
Texture2D nmap : register(t2);

SamplerState splr;


float4 main(float3 viewFragPos : Position,
float3 viewNormal : Normal, 
float3 viewTan : Tangent, 
float3 viewBitan : Bitangent, 
float2 tc : Texcoord) : SV_Target
{
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);
    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
        viewNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
    }
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
    // specular
    const float3 specular = Speculate(
        diffuseColor, diffuseIntensity, viewNormal,
        lv.vToL, viewFragPos, att, specularPower
    );
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(splr, tc);
    clip(texColor.a - 0.1f);
    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specular;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
}