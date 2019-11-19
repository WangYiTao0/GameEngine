#include "Transform.hlsli"
struct VS_INPUT
{
    float3 pos : Position;
    float3 normal : Normal;
};

struct VSOut
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float4 pos : SV_Position;
};

VSOut main(VS_INPUT input)
{
    VSOut vso;
    matrix modelView = mul(world, view);
    matrix modelViewProj = mul(modelView, proj);
    vso.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    vso.viewNormal = mul(input.normal, (float3x3) modelView);
    vso.pos = mul(float4(input.pos, 1.0f), modelViewProj);
    return vso;
}