#include "ShaderOptions.hlsli"
#include "LightVectorData.hlsli"
#include "LightOptions.hlsli"

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf
{
    float specularPowerConst;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);

SamplerState splr;


float4 main(PS_INPUT input) : SV_Target
{
    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
    // specular parameters
    float specularPower = specularPowerConst;
    const float4 specularSample = spec.Sample(splr, input.texcoord);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
    if (hasGloss)
    {
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse light
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.viewNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, input.viewNormal,
        lv.vToL, input.viewPixelPos, att, specularPower
    );
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(splr, input.texcoord);
      //clip(texColor.a - 0.1f);
    clip(texColor.a < 0.1f ? -1 : 1);
    // flip Normal when backface
    if (dot(input.viewNormal, input.viewPixelPos) >= 0.0f)
    {
        input.viewNormal = -input.viewNormal;
    }
    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specularReflected;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
}