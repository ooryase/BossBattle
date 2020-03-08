#include"InputController.h"

InputController& InputController::getInstance()
{
	static InputController instance;
	return instance;
}

HRESULT InputController::Init(HINSTANCE hInstance, HWND hWnd)
{
	pad.insert(std::make_pair(XINPUT_GAMEPAD_DPAD_UP		, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_DPAD_DOWN		, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_DPAD_LEFT		, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_DPAD_RIGHT		, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_START			, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_BACK			, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_LEFT_THUMB		, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_RIGHT_THUMB	, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_LEFT_SHOULDER	, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_RIGHT_SHOULDER	, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_A				, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_B				, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_X				, PressData()));
	pad.insert(std::make_pair(XINPUT_GAMEPAD_Y				, PressData()));

	thumbLeft.insert(std::make_pair(THUMB_LEFT::LEFT, PressData()));
	thumbLeft.insert(std::make_pair(THUMB_LEFT::RIGHT, PressData()));
	thumbLeft.insert(std::make_pair(THUMB_LEFT::UP, PressData()));
	thumbLeft.insert(std::make_pair(THUMB_LEFT::DOWN, PressData()));

	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)lpDI.GetAddressOf(), NULL);
	if (FAILED(hr))
		return hr;

	hr = lpDI->CreateDevice(GUID_SysKeyboard, lpKeyboard.GetAddressOf(), NULL);
	if (FAILED(hr))
		return hr;

	hr = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;

	hr = lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr))
		return hr;

	lpKeyboard->Acquire();

	ZeroMemory(key, sizeof(key));

	return S_OK;
}

void InputController::Update()
{
	static XINPUT_STATE state;

	XInputGetState(0, &state);

	for (auto&& var : pad)
	{
		var.second.Update(state.Gamepad.wButtons & var.first);
	}

	memcpy(prevKey, key, sizeof(BYTE) * 256);

	HRESULT hr = lpKeyboard->GetDeviceState(sizeof(key), key);
	if (FAILED(hr))
	{
		lpKeyboard->Acquire();
		lpKeyboard->GetDeviceState(sizeof(key), key);
	}

	thumbLeft.at(THUMB_LEFT::LEFT).Update(state.Gamepad.sThumbLX < -0.5f);
	thumbLeft.at(THUMB_LEFT::RIGHT).Update(state.Gamepad.sThumbLX > 0.5f);
	thumbLeft.at(THUMB_LEFT::UP).Update(state.Gamepad.sThumbLY > 0.5f);
	thumbLeft.at(THUMB_LEFT::DOWN).Update(state.Gamepad.sThumbLY < -0.5f);
}

void InputController::Release()
{
	lpKeyboard.Reset();
	lpDI.Reset();
}

bool InputController::IsPressKey(int keyCode)
{
	return key[keyCode] & 0x80;
}

bool InputController::IsPressButtom(int Xcode)
{
	return pad.at(Xcode).press;
}

bool InputController::IsPushKey(int keyCode)
{
	return (key[keyCode] & 0x80) && !(prevKey[keyCode] & 0x80);
}

bool InputController::IsPushButtom(int Xcode)
{
	return pad.at(Xcode).push;
}

bool InputController::IsReleaseKey(int keyCode)
{
	return !(key[keyCode] & 0x80) && (prevKey[keyCode] & 0x80);
}

bool InputController::IsReleaseButtom(int Xcode)
{
	return pad.at(Xcode).release;
}

bool InputController::IsPressThumbL(THUMB_LEFT _code)
{
	return thumbLeft.at(_code).press;
}

bool InputController::IsPushThumbL(THUMB_LEFT _code)
{
	return thumbLeft.at(_code).push;
}

bool InputController::IsReleaseThumbL(THUMB_LEFT _code)
{
	return thumbLeft.at(_code).release;
}

void PressData::Update(bool nowPress)
{
	if (nowPress)
	{
		push = nowPress && !press;
		release = false;
	}
	else
	{
		push = false;
		release = !nowPress && press;
	}
	press = nowPress;
}