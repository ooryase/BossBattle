#include"Resource.h"
#include"DeviceManager.h"
#include"InputController.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR pCmdLine, int nCmdShow)
{
	//ウィンドウクラスを登録
	const char* WndClassName = "D3D11syokika";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WndClassName;
//	wc.lpszMenuName = "MENUBAR";
	if (!RegisterClass(&wc))
	{
		return S_FALSE;
	}

	//ウィンドウの作成
	const int CWIDTH = 1280;
	const int CHEIGHT = 720;
	HWND WHandle = CreateWindow(WndClassName, WndClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CWIDTH, CHEIGHT, NULL, NULL, hInstance, NULL);
	if (WHandle == NULL)
	{
		MessageBox(WHandle, _T("InitApp"), _T("Err"), MB_ICONSTOP);
		return S_FALSE;
	}

	std::unique_ptr<DeviceManager> deviceManager = std::make_unique<DeviceManager>(WHandle);
	if (FAILED(deviceManager->HResult))
	{
		deviceManager.release();
		MessageBox(WHandle, _T("DeviceManager"), _T("Err"), MB_ICONSTOP);
		return S_FALSE;
	}

	InputController::getInstance().Init(hInstance, WHandle);

	// ウインドウ表示
	ShowWindow(WHandle, SW_SHOWNORMAL);
	UpdateWindow(WHandle);

	// メッセージループの実行
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InputController::getInstance().Update();

			if (InputController::getInstance().IsPressKey(DIK_A))
			{
				std::cout << "iketaaaa" << std::endl;
			}





			deviceManager->RenderBegin();

			deviceManager->RenderEnd();
		}
	}


	UnregisterClass(WndClassName, hInstance);

	InputController::getInstance().Release();
	deviceManager.release();

	return (int)msg.wParam;

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	// デフォルト処理
	return DefWindowProc(hWnd, msg, wParam, lParam);

}
