
#ifndef _SHADING_MATH_H
#define _SHADING_MATH_H

#define USE_BIT_MANIPULATION	
#define PI 3.14159265359f
#define TWO_PI 6.28318530718f
#define PI_OVER_TWO 1.5707963268f
#define PI_SQUARED 9.86960440109f

inline float3 UnpackNormals(Texture2D normalMap, SamplerState normalSampler, float2 uv, float3 worldNormal, float3 worldTangent)
{
    // uncompressed normal in tangent space
    float3 SampledNormal = normalMap.Sample(normalSampler, uv).xyz;
    SampledNormal = normalize(SampledNormal * 2.0f - 1.0f);

    const float3 T = normalize(worldTangent - dot(worldNormal, worldTangent) * worldNormal);
    const float3 N = normalize(worldNormal);
    const float3 B = normalize(cross(T, N));
    const float3x3 TBN = float3x3(T, B, N);
    return mul(SampledNormal, TBN);
}

#ifdef USE_BIT_MANIPULATION
float RadicalInverse_VdC(uint bits) //  Van Der Corpus
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
#else
// the non-bit-manipulation version of the above function
float VanDerCorpus(uint n, uint base)
{
    float invBase = 1.0 / float(base);
    float denom = 1.0;
    float result = 0.0;

    for (uint i = 0u; i < 32u; ++i)
    {
        if (n > 0u)
        {
            denom = n % 2.0f;
            result += denom * invBase;
            invBase = invBase / 2.0;
            n = uint(float(n) / 2.0);
        }
    }

    return result;
}
#endif

float2 Hammersley(int i, int count)
{
#ifdef USE_BIT_MANIPULATION
    return float2(float(i) / float(count), RadicalInverse_VdC(uint(i)));
#else
	// note: this crashes for some reason.
    return float2(float(i) / float(count), VanDerCorpus(uint(i), 2u));
#endif
}

inline float LinearDepth(in float zBufferSample, in float A, in float B)
{

    return A / (zBufferSample - B);
}
inline float LinearDepth(in float zBufferSample, in matrix matProjInverse)
{
    return LinearDepth(zBufferSample, matProjInverse[2][3], matProjInverse[2][2]);
}

float3 ViewSpacePosition(in const float nonLinearDepth, const in float2 uv, const in matrix invProjection)
{

    const float x = uv.x * 2 - 1; // [-1, 1]
    const float y = (1.0f - uv.y) * 2 - 1; // [-1, 1]
    const float z = nonLinearDepth; // [ 0, 1]

    float4 projectedPosition = float4(x, y, z, 1);
    float4 viewSpacePosition = mul(invProjection, projectedPosition);
    return viewSpacePosition.xyz / viewSpacePosition.w;
}

#endif