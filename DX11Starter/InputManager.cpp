#include "InputManager.h"

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

bool InputManager::GetKey(int keyCode)
{
	return GetAsyncKeyState(keyCode) & 0x8000;
}

bool InputManager::GetKeyDown(int keyCode)
{
	bool stateThisFrame = GetKey(keyCode);
	bool toReturn = !keysPressedLastFrame[keyCode] && stateThisFrame;
	keysPressedLastFrame[keyCode] = stateThisFrame;
	return toReturn;
}
