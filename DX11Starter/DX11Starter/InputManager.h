#pragma once

#include <utility>
#include <DirectXMath.h>
#include <map>

class Input {

public:

	struct MouseDelta {
		float x;
		float y;
	};

	static Input* Instance() {
		if (!instance)
			instance = new Input;
		return instance;
	}

	bool GetKey(int key);
	bool GetKeyDown(int key);
	bool GetKeyUp(int key);

	void SetMouseState(int _button, bool _state);
	bool GetMouse(int button);
	bool GetMouseDown(int button);
	bool GetMouseUp(int button);
	DirectX::XMFLOAT2 GetMousePosition();
	DirectX::XMFLOAT2 GetMouseDelta();

	void UpdateMousePosition(float _x, float _y);
	void EndFrame();

private:

	static Input *instance;

	Input();

	std::map<int, bool> mouseState;

	DirectX::XMFLOAT2 prevMousePosition;
	DirectX::XMFLOAT2 currMousePosition;
	DirectX::XMFLOAT2 mouseDelta;

};
