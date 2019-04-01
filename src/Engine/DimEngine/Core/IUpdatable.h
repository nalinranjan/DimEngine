#ifndef IUPDATABLE_H
#define IUPDATABLE_H
#pragma once

#include "../Common/Typedefs.h"

namespace DimEngine
{
	struct IUpdatable
	{
	private:
		virtual void Update(f32 deltaTime, f32 currentTime) = 0;
		virtual void LateUpdate(f32 deltaTime, f32 currentTime) = 0;
	};
}
#endif // !IUPDATABLE_H