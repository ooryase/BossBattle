#include"DeviceManager.h"

DeviceManager::DeviceManager(HWND WHandle)
{
	CoInitialize(NULL);

	//ウィンドウのクライアントエリア
	RECT rc;
	GetClientRect(WHandle, &rc);
	UINT CWIDTH;
	CWIDTH = rc.right - rc.left;
	UINT CHEIGHT;
	CHEIGHT = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC dscd;
	ZeroMemory(&dscd, sizeof(dscd));    // 構造体の値を初期化(必要な場合)
	dscd.BufferCount = 1;
	dscd.BufferDesc.Width = CWIDTH;
	dscd.BufferDesc.Height = CHEIGHT;
	dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dscd.BufferDesc.RefreshRate.Numerator = 60;
	dscd.BufferDesc.RefreshRate.Denominator = 1;
	dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dscd.OutputWindow = WHandle;
	dscd.SampleDesc.Count = 1;
	dscd.SampleDesc.Quality = 0;
	dscd.Windowed = TRUE;
	dscd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL fl;
	fl = D3D_FEATURE_LEVEL_11_0;
	HResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &dscd, &pSwapChain, &pDevice, NULL, &pDeviceContext);
	if (FAILED(HResult)) return;

	//バックバッファの初期化
	ID3D11Texture2D* pBackBuffer;
	HResult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(HResult)) return;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);
	HResult = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	if (FAILED(HResult)) return;

	//深度、ステンシル　テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HResult = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(HResult)) return;

	//深度、ステンシル　ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	HResult = pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	if (FAILED(HResult)) return;

	//ビューポートの設定
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].Width = (float)CWIDTH;
	viewPort[0].Height = (float)CHEIGHT;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;
}

DeviceManager::~DeviceManager()
{
	SAFE_RELEASE(pDepthStencilView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pDeviceContext);
	SAFE_RELEASE(pDevice);

	CoUninitialize();
}

void DeviceManager::RenderBegin()
{
	// 描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);

	// 深度/ステンシル値のクリア
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ラスタライザにビューポートを設定
	pDeviceContext->RSSetViewports(1, viewPort);

	// 描画ターゲット・ビューを出力マージャーの描画ターゲットとして設定
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

}

void DeviceManager::RenderEnd()
{
	pSwapChain->Present(0, 0);
}
