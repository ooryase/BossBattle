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

	lpDI = NULL;
	lpKeyboard = NULL;

	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpDI, NULL);
	if (FAILED(hr))
		return hr;

	hr = lpDI->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL);
	if (FAILED(hr))
	{
		//lpDI->Release();
		return hr;
	}

	hr = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		//lpKeyboard->Release();
		//lpDI->Release();
		return hr;
	}

	hr = lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		//lpKeyboard->Release();
		//lpDI->Release();
		return hr;
	}

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

	//BYTE key[256];
	HRESULT hr = lpKeyboard->GetDeviceState(sizeof(key), key);
	if (FAILED(hr))
	{
		lpKeyboard->Acquire();
		lpKeyboard->GetDeviceState(sizeof(key), key);
	}

}

void InputController::Release()
{
	SAFE_RELEASE(lpKeyboard);
	SAFE_RELEASE(lpDI);
}

bool InputController::IsPressKey(int keyCode)
{
	return key[keyCode] & 0x80;
}

bool InputController::IsPressButtom(int Xcode)
{
	return pad.at(Xcode).press;
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