#include "CommonPSOption.hlsli"

struct VS_INPUT
{
    float3 pos : Position;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 biTangent : Bitangent;
    float2 texcoord : Texcoord;
};

struct VS_OUTPUT
{
    float3 viewPos : Position;
    float3 viewNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float2 tc : Texcoord;
    float4 pos : SV_Position;
};

 VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vso;
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);

    vso.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    vso.viewNormal = mul(input.normal, (float3x3) modelView);
    vso.tan = mul(input.tangent, (float3x3) modelView);
    vso.bitan = mul(input.biTangent, (float3x3) modelView);
    vso.pos = mul(float4(input.pos, 1.0f), modelViewprojMatrix);
    vso.tc = input.texcoord;
    return vso;
}