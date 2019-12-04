cbuffer TransformCBuf : register(b0)
{
    matrix worldMatrix;
    matrix worldInvTranspose;
    matrix viewMatrix;
    matrix projMatrix;
    matrix orthoMatrix;
    matrix worldMatrix2D;
};

SamplerState sample0 : register(s0); //liner
//sampler sample1 : register(s1); //ANISOTROPIC
