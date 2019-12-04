#include "ShadingMath.hlsli"

// constants
#define EPSILON 0.000000000001f

// defines
#define _DEBUG

struct Phong_Surface
{
    float3 N;
    float3 diffuseColor;
    float3 specularColor;
    float shininess;
};
// returns diffuse and specular components of phong illumination model
float3 Phong(Phong_Surface s,float3 L ,float3 V, float3 lightColor)
{
    const float3 N = s.N;
    //reflection
    const float3 R = normalize(2 * N * dot(N, L) - L);
    float diffuse = max(0.0f, dot(N, L));
    float3 Id = lightColor * s.diffuseColor * diffuse;
#ifdef BLINN_PHONG
	const float3 H = normalize(L + V);
	float3 Is = lightColor * s.specularColor * pow(max(dot(N, H), 0.0f), 4.0f * s.shininess) * diffuse;
#else
    float3 Is = lightColor * s.specularColor * pow(max(dot(R, V), 0.0f), s.shininess) * diffuse;
#endif

	//float3 Is = light.color * pow(max(dot(R, V), 0.0f), 240) ;
    return Id + Is;
}
