#include "CommonCbuf.hlsli"

struct PS_IN
{
    float4 position : SV_Position;
    float2 depthTexcoord : TEXCOORD0;
};

Texture2D depthTexture : TEXTURE : register(t0);

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * s_nearZ * s_farZ) / (s_farZ + s_nearZ - z * (s_farZ - s_nearZ));
}



SamplerState sample0 : register(s0);

float4 main(PS_IN pIn) : SV_TARGET
{
    float depthValue;
    float4 color;

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = depthTexture.Sample(sample0, pIn.depthTexcoord).r;
    float pro_depthValue = LinearizeDepth(depthValue) / s_farZ;
    color = float4(float3(pro_depthValue, pro_depthValue, pro_depthValue), 1.0); // perspective
   // color = float4(depthValue, depthValue, depthValue, 1.0f);// orthographic

    return color;
}