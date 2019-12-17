#include "PBRScene.h"

PBRScene::PBRScene(Graphics& gfx)
	:
	gfx(gfx),
	light(gfx),
	Scene("PBRScene")
{


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

    // render light source (simply re-render sphere at light positions)
    // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
    // keeps the codeprint small.
    for (unsigned int i = 0; i < 4; ++i)
    {
        
   
    }

}

PBRScene::~PBRScene()
{
}

void PBRScene::Update(float dt)
{
    light.Bind(gfx);
}

void PBRScene::Draw()
{
    for (auto& p : pbrBall)
    {
        p->DrawIndexed(gfx);
    }
}
