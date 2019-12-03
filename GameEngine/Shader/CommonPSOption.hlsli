cbuffer TransformCBuf : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projMatrix;
    matrix orthoMatrix;
    matrix worldMatrix2D;
};
cbuffer PointLightCBuf : register(b1)
{
    float3 worldMatrixLightPos;
    float diffuseIntensity;
    float3 ambient; 
    float attConst;
    float3 diffuseColor;
    float attLin;
    float attQuad;
};

sampler sample0 : register(s0); //liner
//sampler sample1 : register(s1); //ANISOTROPIC
