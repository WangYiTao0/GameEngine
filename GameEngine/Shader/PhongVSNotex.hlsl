#include "CommonCbuf.hlsli"

struct VS_INPUT
{
    float3 pos : Position;
    float3 normal : Normal;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float4 pos : SV_Position;
};

VSOut main(VS_INPUT input)
{
    VSOut vso;
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);
    vso.worldPos = (float3) mul(float4(input.pos, 1.0f), worldMatrix);
    vso.worldNormal = mul(input.normal, (float3x3) worldInvTranspose);
    vso.pos = mul(float4(input.pos, 1.0f), modelViewprojMatrix);
    return vso;
}