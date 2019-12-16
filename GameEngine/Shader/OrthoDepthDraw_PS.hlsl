#include "CommonCbuf.hlsli"

struct PS_IN
{
    float4 position : SV_Position;
    float2 depthTexcoord : TEXCOORD0;
};

Texture2D depthTexture : TEXTURE : register(t0);

float LinearizeDepth(float depth)
{
    float near = 0.1f;
    float far = 4000;

    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}



SamplerState sample0 : register(s0);

float4 main(PS_IN pIn) : SV_TARGET
{
    float depthValue;
    float4 color;

    float near = 0.1f;
    float far = 4000;

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = depthTexture.Sample(sample0, pIn.depthTexcoord).r;
    float pro_depthValue = LinearizeDepth(depthValue) / far;
    //color = float4(float3(pro_depthValue, pro_depthValue, pro_depthValue), 1.0); // perspective
    color = float4(depthValue, depthValue, depthValue, 1.0f);// orthographic

    return color;
}