#include "PBRScene.h"

PBRScene::PBRScene(Graphics& gfx)
	:
	gfx(gfx),
	light(gfx),
	Scene("PBRScene")
{

    // init Light
    m_Light = std::make_unique<Light>(gfx, "PBRScene", 1, 4, 4);

    std::vector<std::string> pbrTexture[5];

    pbrTexture[0] = { 
    "Data\\Images\\PBR\\rusted_iron\\albedo.png",
 "Data\\Images\\PBR\\rusted_iron\\normal.png",
 "Data\\Images\\PBR\\rusted_iron\\metallic.png",
 "Data\\Images\\PBR\\rusted_iron\\roughness.png",
 "Data\\Images\\PBR\\rusted_iron\\ao.png" };
    pbrTexture[1] = {
"Data\\Images\\PBR\\gold\\albedo.png",
"Data\\Images\\PBR\\gold\\normal.png",
"Data\\Images\\PBR\\gold\\metallic.png",
"Data\\Images\\PBR\\gold\\roughness.png",
"Data\\Images\\PBR\\gold\\ao.png" };
    pbrTexture[2] = {
"Data\\Images\\PBR\\plastic\\albedo.png",
"Data\\Images\\PBR\\plastic\\normal.png",
"Data\\Images\\PBR\\plastic\\metallic.png",
"Data\\Images\\PBR\\plastic\\roughness.png",
"Data\\Images\\PBR\\plastic\\ao.png" };
    pbrTexture[3] = {
"Data\\Images\\PBR\\grass\\albedo.png",
"Data\\Images\\PBR\\grass\\normal.png",
"Data\\Images\\PBR\\grass\\metallic.png",
"Data\\Images\\PBR\\grass\\roughness.png",
"Data\\Images\\PBR\\grass\\ao.png" };
    pbrTexture[4] = {
"Data\\Images\\PBR\\wall\\albedo.png",
"Data\\Images\\PBR\\wall\\normal.png",
"Data\\Images\\PBR\\wall\\metallic.png",
"Data\\Images\\PBR\\wall\\roughness.png",
"Data\\Images\\PBR\\wall\\ao.png" };

    for (int i = 0; i < nrRows * nrColumns; i++)
    {
        shaderBall.push_back(std::make_unique<Model>(gfx,
            "Data\\Models\\PBR\\shaderBall.obj", 1.0f / 80.0f,pbrTexture[1]));
    }
    
    int i = 0;
    for (int row = 1; row < nrRows + 1; ++row)
    {

        for (int col = 1; col < nrColumns + 1; ++col)
        {
            shaderBall[i]->SetRootTransform(
                {
                    DirectX::XMMatrixTranslation(
                (float)(col - ((nrColumns + 1) / 2)) * spacing,
                (float)(row - ((nrRows + 1) / 2)) * spacing,
                0.0f )
                });
            i++;
        }
    }

    //std::vector<std::string> filePath = {
    //"Data\\Images\\skybox\\Yokohama3\\posx.jpg", "Data\\Images\\skybox\\Yokohama3\\negx.jpg",
    //"Data\\Images\\skybox\\Yokohama3\\posy.jpg", "Data\\Images\\skybox\\Yokohama3\\negy.jpg",
    //"Data\\Images\\skybox\\Yokohama3\\posz.jpg", "Data\\Images\\skybox\\Yokohama3\\negz.jpg" };


    skyHdr = std::make_unique<SkyRender>(gfx, "Data\\Images\\hdr\\HdrCubeMap.hdr", 1000,true);

}

PBRScene::~PBRScene()
{
}

void PBRScene::Update(float dt)
{
    //update light
    m_Light->Update(gfx);
    m_Light->Bind(gfx);

    light.Bind(gfx);

    m_Light->SpawnLightManagerWindow(gfx);
}

void PBRScene::Draw()
{
    m_Light->Draw(gfx);

    for (auto& b : shaderBall)
    {
       b->Draw(gfx);
    }
    //for (auto& p : pbrBall)
    //{
    //    p->DrawIndexed(gfx);
    //}

    skyHdr->DrawIndexed(gfx);
}
