#include "ShaderOptions.hlsli"
#include "LightVectorData.hlsli"
#include "LightOptions.hlsli"

struct PS_INPUT
{
    //SV_Position describes the pixel location.
    float3 viewPixelPos : Position;
    float3 viewNormal : Normal;
    float3 viewTan : Tangent;
    float3 viewBitan : Bitangent;
    float2 texcoord : Texcoord;
};

cbuffer ObjectCBuf
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);

SamplerState splr : register(s0);


float4 main(PS_INPUT input) : SV_Target
{
    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
        input.viewNormal = MapNormal(normalize(input.viewTan), normalize(input.viewBitan), input.viewNormal, input.texcoord, nmap, splr);
    }
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    //if doesn't has specularMap  using specularPowerConst
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, input.texcoord);
        specularReflectionColor = specularSample.rgb * specularMapWeight;

        //specularSample.a is [0,1] need  be power scale
        //specularPower = pow(2.0, 13.0 * shininessMap)
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
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

    //alpha blender
    float4 texColor = tex.Sample(splr, input.texcoord);
    //clip(texColor.a - 0.1f);
    clip(texColor.a < 0.1f ? -1 : 1);
    finalColor.rgb = texColor.rgb * saturate(ambient + diffuse) + specularReflected;
    finalColor.a = texColor.a;

	// final color
    return finalColor;
}
