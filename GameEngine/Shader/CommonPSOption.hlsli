cbuffer TransformCBuf : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
    matrix ortho;
    matrix world2D;
};
cbuffer PointLightCBuf : register(b1)
{
    float3 worldLightPos;
    float diffuseIntensity;
    float3 ambient; 
    float attConst;
    float3 diffuseColor;
    float attLin;
    float attQuad;
};

sampler sample0 : register(s0); //liner
//sampler sample1 : register(s1); //ANISOTROPIC
