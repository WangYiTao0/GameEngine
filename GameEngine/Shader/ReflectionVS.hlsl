#include "CommonCbuf.hlsli"

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

      // Calculate the position of the vertex against the worldMatrix, view, and projMatrixection matrices.
    vOut.pos = mul(float4(vIn.pos, 1.0f), worldMatrix);
    vOut.pos = mul(vOut.pos, viewMatrix);
    vOut.pos = mul(vOut.pos, projMatrix);

   // Store the texture coordinates for the pixel shader.
    vOut.tex = vIn.tex;
   // Create the reflection projMatrixection worldMatrix matrix.
    reflectProjectWorld = mul(reflectionMatrix, projMatrix);
    reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

    // Calculate the input position against the reflectprojMatrixectworldMatrix matrix.
    vOut.reflectionPosition = mul(float4(vIn.pos, 1.0f), reflectProjectWorld);

    return vOut;
}