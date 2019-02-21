#include "InputManager.h"
#include <Windows.h>
#include <iostream>
// Initialize input manager
Input* Input::instance = 0;

bool Input::GetKey(int key) {
	return (GetAsyncKeyState(key) & 0x8000);
}

bool Input::GetKeyDown(int key) {
	return false;
}

bool Input::GetKeyUp(int key) {
	return false;
}

void Input::SetMouseState(int _button, bool _state) {
	mouseState[_button] = _state;
}

bool Input::GetMouse(int _button) {
	return mouseState[_button];
}

bool Input::GetMouseDown(int button) {
	return false;
}

bool Input::GetMouseUp(int button) {
	return false;
}

DirectX::XMFLOAT2 Input::GetMousePosition() {
	return currMousePosition;
}

DirectX::XMFLOAT2 Input::GetMouseDelta() {
	return mouseDelta;
}

void Input::UpdateMousePosition(float _x, float _y) {
	currMousePosition.x = _x;
	currMousePosition.y = _y;
}

void Input::EndFrame() {
	mouseDelta.x = currMousePosition.x - prevMousePosition.x;
	mouseDelta.y = currMousePosition.y - prevMousePosition.y;
	prevMousePosition = currMousePosition;
}

Input::Input() {
	prevMousePosition = currMousePosition;
}