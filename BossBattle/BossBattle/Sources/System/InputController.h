#pragma once
#include"Resource.h"

#define DIRECTINPUT_VERSION 0x800
#include<unordered_map>
#include<Xinput.h>
#include<dinput.h>

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct PressData
{
	bool press;
	bool push;
	bool release;

	PressData() :press(false), push(false), release(false) {}

	void Update(bool nowPress);
};

class InputController
{
private:
	InputController(){}
	InputController(const InputController&) = delete;
	InputController& operator = (const InputController&) = delete;

	std::unordered_map<int, PressData>keys;
	std::unordered_map<int, PressData>pad;

	LPDIRECTINPUT8 lpDI;
	LPDIRECTINPUTDEVICE8 lpKeyboard;

	BYTE key[256];

public:
	static InputController& getInstance();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update();
	void Release();

	bool IsPressKey(int keyCode);
	bool IsPressButtom(int Xcode);

};