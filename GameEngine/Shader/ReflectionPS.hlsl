#include "CommonCbuf.hlsli"

Texture2D shaderTexture;
Texture2D reflectionTexture;

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};


SamplerState sample0 : register(s0);

float4 main(PS_IN pIn):SV_TARGET
{
    float4 textureColor;
    float2 reflectTexCoord;
    float4 reflectionColor;
    float4 color;

    // Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(sample0, pIn.tex);
    
        // Calculate the projMatrixected reflection texture coordinates.
    reflectTexCoord.x = pIn.reflectionPosition.x / pIn.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -pIn.reflectionPosition.y / pIn.reflectionPosition.w / 2.0f + 0.5f;

    // Sample the texture pixel from the reflection texture using the projMatrixected texture coordinates.
    reflectionColor = reflectionTexture.Sample(sample0, reflectTexCoord);

    // Do a linear interpolation between the two textures for a blend effect.
    color = lerp(textureColor, reflectionColor, 0.15f);

    return color;

}

