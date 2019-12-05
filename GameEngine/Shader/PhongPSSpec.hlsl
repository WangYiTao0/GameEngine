#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf : register(b2)
{
    float specularPowerConst;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);



float4 main(PS_INPUT input) : SV_Target
{
    // normalize the mesh normal
    input.worldNormal = normalize(input.worldNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(worldLightPos, input.worldPos);
    // specular parameters
    float specularPower = specularPowerConst;
    const float4 specularSample = spec.Sample(sample0, input.texcoord);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
    if (hasGloss)
    {
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse light
    const float3 diff = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.worldNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, input.worldNormal,
        lv.vToL, input.worldPos, cameraPos, att, specularPower
    );
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    float4 finalColor = 1.0f;
    float4 texColor = tex.Sample(sample0, input.texcoord);
      //clip(texColor.a - 0.1f);
    clip(texColor.a < 0.1f ? -1 : 1);

    finalColor.rgb = texColor.rgb * saturate(ambient + diff) + specularReflected;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
}