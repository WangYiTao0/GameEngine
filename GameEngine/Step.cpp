#include "Step.h"
#include "Drawable.h"
#include "FrameCommander.h"

void Step::Submit(class FrameCommander& frame, const class Drawable& drawable) const
{
	frame.Accept(Job{ this,&drawable }, targetPass);
}

void Step::InitializeParentReferences(const class Drawable& parent) noexcept
{
	for (auto& b : bindables)
	{
		b->InitializeParentReference(parent);
	}
}
