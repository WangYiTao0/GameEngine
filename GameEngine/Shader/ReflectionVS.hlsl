#include "CommonPSOption.hlsli"

cbuffer ReflectionMatrix
{
    matrix reflectionMatrix;
};

struct VS_IN
{
    float3 pos : Postion;
    float2 tex : Texcoord;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};

VS_OUT main(VS_IN vIn)
{
    VS_OUT vOut;
    matrix reflectProjectWorld;

      // Calculate the position of the vertex against the world, view, and projection matrices.
    vOut.pos = mul(float4(vIn.pos, 1.0f), world);
    vOut.pos = mul(vOut.pos, view);
    vOut.pos = mul(vOut.pos, proj);

   // Store the texture coordinates for the pixel shader.
    vOut.tex = vIn.tex;
   // Create the reflection projection world matrix.
    reflectProjectWorld = mul(reflectionMatrix, proj);
    reflectProjectWorld = mul(world, reflectProjectWorld);

    // Calculate the input position against the reflectProjectWorld matrix.
    vOut.reflectionPosition = mul(vIn.pos, reflectProjectWorld);

    return vOut;
}