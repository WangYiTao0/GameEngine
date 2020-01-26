#include "Job.h"
#include "Step.h"
#include "Drawable.h"

Job::Job(const class Step* pStep, const class Drawable* pDrawable)
	: pDrawable{ pDrawable },
	pStep{ pStep }
{

}

void Job::Execute(class Graphics& gfx) const noxnd
{
	pDrawable->Bind(gfx);
	pStep->Bind(gfx);
	gfx.DrawIndexed(pDrawable->GetIndexCount());
}
