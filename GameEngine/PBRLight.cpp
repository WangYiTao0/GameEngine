#include "PBRLight.h"

PBRLight::PBRLight(Graphics& gfx)
	:
	cbuf(gfx, 5u)
{
	for (int i = 0; i < 4; i++)
	{
		lightData.position[i] = { 5.0f * (i-2),0.0f,-10.0f,1.0f };

	
		lightData.color[i] = { 300.0f, 300.0f, 300.0f,300.0f };
	}
}

void PBRLight::Bind(Graphics& gfx)
{
	cbuf.Update(gfx, lightData);
	cbuf.Bind(gfx);
}
