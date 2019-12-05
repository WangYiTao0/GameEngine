#include "CommonPSOption.hlsli"

struct VS_INPUT
{
    float3 pos : Position;
    float3 normal : Normal;
    float2 texcoord : Texcoord;
};

struct VS_OUTPUT
{
    float3 worldPos : Position;
    float3 worldNormal : Normal;
    float2 tc : Texcoord;
    //SV_Position describes the pixel location.
    float4 pos : SV_Position;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT vso;

    matrix modelView = mul(worldMatrix, viewMatrix);
    matrix modelViewprojMatrix = mul(modelView , projMatrix);

    //Camera view Positon
    vso.worldPos = (float3) mul(float4(input.pos, 1.0f), worldMatrix);
    //Camera view Normal
    vso.worldNormal = mul(input.normal, (float3x3) worldInvTranspose);
    //worldMatrix pos
    vso.pos = mul(float4(input.pos, 1.0f), modelViewprojMatrix);
    vso.tc = input.texcoord;
    return vso;
}