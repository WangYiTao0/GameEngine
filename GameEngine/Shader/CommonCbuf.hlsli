
//VS stage PS stage
cbuffer TransformCBuf : register(b0)
{
    matrix worldMatrix;
    matrix worldInvTranspose;
    matrix viewMatrix;
    matrix viewInvTranspose;
    matrix projMatrix;

    matrix orthoMatrix;
    matrix worldMatrix2D;

    float3 cameraPos;
    float cameraPadding;
};

//VS stage
cbuffer ShadowCB :register(b1)
{
    matrix s_View;
    matrix s_Proj;
    matrix s_Ortho;
}


