#include "CommonPSOption.hlsli"

struct PS_IN
{
    float3 posL : POSITION;
};

TextureCube texCube : register(t4);

float4 main(PS_IN pIn) : SV_Target
{
    return texCube.Sample(sample0, pIn.posL);
}