#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
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

cbuffer ObjectCBuf : register(b2)
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D diff : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);



float4 main(PS_INPUT input) : SV_Target
{
    
    //alpha blender
    float4 diffColor = diff.Sample(sample0, input.texcoord);
	//using view light pos  
    float3 viewLightPos = mul(float4(worldLightPos, 1.0f), viewMatrix);
    #ifdef HASMASK
    // bail if highly translucent
    clip(diffColor.a < 0.1f ? -1 : 1);
    // flip normal when backface
    if (dot(input.viewNormal, input.viewPixelPos) >= 0.0f)
    {
        input.viewNormal = -input.viewNormal;
    }
    #endif
    // normalize the mesh normal
    input.viewNormal = normalize(input.viewNormal);
    // replace normal with mapped if normal mapping enabled
    if (normalMapEnabled)
    {
        input.viewNormal = MapNormal(normalize(input.viewTan), normalize(input.viewBitan), input.viewNormal, input.texcoord, nmap, sample0);
    }
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, input.viewPixelPos);
    // specular parameter determination (mapped or uniform)
    float3 specularReflectionColor;
    //if doesn't has specularMap  using specularPowerConst
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(sample0, input.texcoord);
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
    const float3 diff = Diffuse(diffuse, intensity, att, lv.dirToL, input.viewNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, input.viewNormal,
        lv.vToL, input.viewPixelPos, att, specularPower
    );
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    float4 finalColor = 1.0f;

    finalColor.rgb = diffColor.rgb * saturate(ambient + diff) + specularReflected;
    finalColor.a = diffColor.a;

	// final color
    return finalColor;
}
