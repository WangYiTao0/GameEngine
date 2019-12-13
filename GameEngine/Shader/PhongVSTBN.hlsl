#include "CommonCbuf.hlsli"

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
    float3 worldPos : Position;
    float3 LightSpacePos : POSITION1;
    float3 worldNormal : Normal;
    float3 worldTan : Tangent;
    float3 worldBitan : Bitangent;
    float2 tc : Texcoord;
    float4 pos : SV_Position;
};

 VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vso;
    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView, projMatrix);

    matrix lightView = mul(worldMatrix, s_View);
    matrix lightSpaceMatrix = mul(lightView, s_Proj);

    vso.worldPos = (float3) mul(float4(input.pos, 1.0f), worldMatrix);
    vso.LightSpacePos = mul(float4(input.pos, 1.0f), lightSpaceMatrix);
    vso.worldNormal = mul(input.normal, (float3x3) worldInvTranspose);
    vso.worldTan = mul(input.tangent, (float3x3) worldMatrix);
    vso.worldBitan = mul(input.biTangent, (float3x3) worldMatrix);
    vso.pos = mul(float4(input.pos, 1.0f), modelViewprojMatrix);
    vso.tc = input.texcoord;
    return vso;
}