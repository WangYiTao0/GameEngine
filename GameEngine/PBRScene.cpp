#include "PBRScene.h"

PBRScene::PBRScene(Graphics& gfx)
	:
	gfx(gfx),
	light(gfx),
	Scene("PBRScene")
{

    // init Light
    m_Light = std::make_unique<Light>(gfx, "PBRScene", 1, 4, 4);

    for (int i = 0; i < nrRows * nrColumns; i++)
    {
        pbrBall.push_back(std::make_unique<PBRBall>(gfx, 1.0f));
    }
    
    int i = 0;
    for (int row = 1; row < nrRows + 1; ++row)
    {

        for (int col = 1; col < nrColumns + 1; ++col)
        {
            pbrBall[i]->SetPos(
                {
                (float)(col - ((nrColumns + 1) / 2)) * spacing,
                (float)(row - ((nrRows + 1) / 2)) * spacing,
                0.0f
                });
            i++;
        }
    }

    std::vector<std::string> filePath = {
    "Data\\Images\\skybox\\Yokohama3\\posx.jpg", "Data\\Images\\skybox\\Yokohama3\\negx.jpg",
    "Data\\Images\\skybox\\Yokohama3\\posy.jpg", "Data\\Images\\skybox\\Yokohama3\\negy.jpg",
    "Data\\Images\\skybox\\Yokohama3\\posz.jpg", "Data\\Images\\skybox\\Yokohama3\\negz.jpg" };


    skyHdr = std::make_unique<SkyRender>(gfx, filePath, 1000);

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

    for (auto& p : pbrBall)
    {
        p->DrawIndexed(gfx);
    }

    skyHdr->DrawIndexed(gfx);
}
