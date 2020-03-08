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

enum class THUMB_LEFT
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class InputController
{
private:
	InputController(){}
	InputController(const InputController&) = delete;
	InputController& operator = (const InputController&) = delete;

	std::unordered_map<int, PressData>keys;
	std::unordered_map<int, PressData>pad;
	std::unordered_map<THUMB_LEFT, PressData>thumbLeft;
	

	ComPtr<IDirectInput8A> lpDI;
	ComPtr<IDirectInputDevice8A> lpKeyboard;

	BYTE key[256];
	BYTE prevKey[256];

public:
	static InputController& getInstance();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update();
	void Release();

	bool IsPressKey(int keyCode);
	bool IsPressButtom(int Xcode);
	bool IsPushKey(int keyCode);
	bool IsPushButtom(int Xcode);
	bool IsReleaseKey(int keyCode);
	bool IsReleaseButtom(int Xcode);
	bool IsPressThumbL(THUMB_LEFT _code);
	bool IsPushThumbL(THUMB_LEFT _code);
	bool IsReleaseThumbL(THUMB_LEFT _code);

};