#pragma once
#include <Windows.h>
#include <map>

class InputManager
{
private:
    std::map<int, bool> keysPressedLastFrame;
public:
    static InputManager& GetInstance();
    bool GetKey(int keyCode);
    bool GetKeyDown(int keyCode);
};

