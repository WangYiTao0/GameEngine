cbuffer TransformCBuf : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};
cbuffer PointLightCBuf : register(b1)
{
    float3 viewLightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

sampler sample0 : register(s0); //liner
//sampler sample1 : register(s1); //ANISOTROPIC
