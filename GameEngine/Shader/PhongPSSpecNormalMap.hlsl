#include "ShaderOptions.hlsli"
#include "LightingUtil.hlsli"
#include "CommonPSOption.hlsli"
#include "ShadingMath.hlsli"

struct PS_INPUT
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

    //float3 viewNormal = mul(input.worldNormal, (float3x3)viewMatrix);
    //float3 viewPos = mul(input.worldPos, (float3x3) viewMatrix);
    //#ifdef HASMASK
    //// bail if highly translucent
    //clip(diffColor.a < 0.1f ? -1 : 1);
    //// flip normal when backface
    ////if (dot(input.viewNormal, input.viewPixelPos) >= 0.0f)
    //if (dot(viewNormal, viewPos) >= 0.0f)
    //{
    //    input.worldNormal = -input.worldNormal;
    //}
    //#endif
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
    const float3 diff = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, input.worldNormal);
    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, input.worldNormal,
        lv.vToL, input.worldPos, cameraPos, att, specularPower
    );
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    float4 finalColor = 1.0f;

    finalColor.rgb = diffColor.rgb * saturate(ambient + diff) + specularReflected;
    finalColor.a = diffColor.a;

	// final color
    return finalColor;
}
