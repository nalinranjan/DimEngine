#include "Global.h"

DimEngine::f32 DimEngine::Global::screenRatio = -1;

DimEngine::f32 DimEngine::Global::GetScreenRatio()
{
	return screenRatio;
}

void DimEngine::Global::SetScreenRatio(f32 value)
{
	screenRatio = value;
}
