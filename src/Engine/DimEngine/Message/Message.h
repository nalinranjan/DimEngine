#pragma once

#include "../Common/Typedefs.h"

namespace DimEngine
{
	enum MessageType : u32
	{
		MESSAGE_MOUSE_DOWN = 0x11,
		MESSAGE_MOUSE_FOCUS = 0x12,
		MESSAGE_MOUSE_UP = 0x14,

		MESSAGE_MOUSE_ENTER = 0x21,
		MESSAGE_MOUSE_STAY = 0x22,
		MESSAGE_MOUSE_EXIT = 0x24,

		MESSAGE_COLLISION_ENTER = 0x41,
		MESSAGE_COLLISION_STAY = 0x42,
		MESSAGE_COLLISION_EXIT = 0x44,
	};

	struct __declspec(dllexport) Message
	{
		MessageType type;
	};
}
