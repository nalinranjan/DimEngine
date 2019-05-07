#ifndef GLOBAL_H
#define GLOBAL_H
#pragma once

#include "../Common/Typedefs.h"

namespace DimEngine
{
	struct __declspec(dllexport) Global
	{
	private:
		static f32 screenRatio;
		static bool useObliqueClipping;
		
	public:
		static f32 GetScreenRatio();
		static void SetScreenRatio(f32 value);

		static bool UseObliqueClipping();
		static void SetObliqueClipping(bool value);
	};
}
#endif