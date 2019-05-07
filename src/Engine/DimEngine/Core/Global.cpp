#include "Global.h"

DimEngine::f32 DimEngine::Global::screenRatio = -1;
bool DimEngine::Global::useObliqueClipping = true;

DimEngine::f32 DimEngine::Global::GetScreenRatio()
{
	return screenRatio;
}

void DimEngine::Global::SetScreenRatio(f32 value)
{
	screenRatio = value;
}

bool DimEngine::Global::UseObliqueClipping()
{
	return useObliqueClipping;
}

void DimEngine::Global::SetObliqueClipping(bool value)
{
	useObliqueClipping = value;
}
