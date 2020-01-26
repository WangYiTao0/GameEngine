#include "Technique.h"

void Technique::Submit(class FrameCommander& frame, const class Drawable& drawable) const noexcept
{
	if (active)
	{
		for (const auto& step : steps)
		{
			step.Submit(frame, drawable);
		}
	}
}

void Technique::InitializeParentReferences(const class Drawable& parent) noexcept
{
	for (auto& s : steps)
	{
		s.InitializeParentReferences(parent);
	}
}
