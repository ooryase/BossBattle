#include"DeviceManager.h"

HRESULT DeviceManager::Init(HWND WHandle)
{
	CoInitialize(NULL);

	HRESULT hr = CreateDeviceAndSwapChain(WHandle);
	if(FAILED(hr))
		return hr;

	hr = CreateBackBuffer();
	if(FAILED(hr))
		return hr;

	hr = CreateBlendState();
	if (FAILED(hr))
		return hr;

	hr = CreateAddBlendState();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void DeviceManager::Release()
{
	pBlendState.Reset();
	pAddBlendState.Reset();
	pDepthStencilView.Reset();
	pDepthStencil.Reset();
	pRenderTargetView.Reset();
	pDeviceContext.Reset();
	pDevice.Reset();
	pSwapChain.Reset();

	CoUninitialize();
}

HRESULT DeviceManager::CreateDeviceAndSwapChain(HWND WHandle)
{
	//ウィンドウのクライアントエリア
	RECT rc;
	GetClientRect(WHandle, &rc);
	UINT CLIENT_WIDTH;
	CLIENT_WIDTH = rc.right - rc.left;
	UINT CLIENT_HEIGHT;
	CLIENT_HEIGHT = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC dscd;
	ZeroMemory(&dscd, sizeof(dscd));    // 構造体の値を初期化(必要な場合)
	dscd.BufferCount = 1;
	dscd.BufferDesc.Width = CLIENT_WIDTH;
	dscd.BufferDesc.Height = CLIENT_HEIGHT;
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
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &dscd, pSwapChain.GetAddressOf(), pDevice.GetAddressOf(), NULL, pDeviceContext.GetAddressOf());
	if (FAILED(hr)) return hr;


	//ビューポートの設定
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].Width = (float)CLIENT_WIDTH;
	viewPort[0].Height = (float)CLIENT_HEIGHT;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;

	return S_OK;
}

HRESULT DeviceManager::CreateBackBuffer()
{
	//バックバッファの初期化
	ComPtr<ID3D11Texture2D> pBackBuffer;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.GetAddressOf());
	pBackBuffer.Reset();
	if (FAILED(hr)) return hr;

	//深度、ステンシル　テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&descDepth, NULL, pDepthStencil.GetAddressOf());
	if (FAILED(hr)) return hr;

	//深度、ステンシル　ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDepthStencilView.GetAddressOf());
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT DeviceManager::CreateBlendState()
{
	D3D11_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++) {
		descBS.RenderTarget[i].BlendEnable = TRUE;
		descBS.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		descBS.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descBS.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		descBS.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		descBS.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		descBS.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBS.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HRESULT hr = pDevice->CreateBlendState(&descBS, pBlendState.GetAddressOf());
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT DeviceManager::CreateAddBlendState()
{
	D3D11_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++) {
		descBS.RenderTarget[i].BlendEnable = TRUE;
		descBS.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		descBS.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		descBS.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		descBS.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		descBS.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		descBS.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBS.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HRESULT hr = pDevice->CreateBlendState(&descBS, pAddBlendState.GetAddressOf());
	if (FAILED(hr)) return hr;

	return S_OK;
}


void DeviceManager::RenderBegin()
{
	// 描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);

	// 深度/ステンシル値のクリア
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ラスタライザにビューポートを設定
	pDeviceContext->RSSetViewports(1, viewPort);

	// 描画ターゲット・ビューを出力マージャーの描画ターゲットとして設定
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());


	//ブレンディングをコンテキストを設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);

}

void DeviceManager::SetBerendState(BLEND_STATE blendState)
{
	ComPtr<ID3D11BlendState> setBlendState;

	switch (blendState)
	{
	case BLEND_STATE::ALIGMENT:
		setBlendState = pBlendState;
		break;
	case BLEND_STATE::ADD:
		setBlendState = pAddBlendState;
		break;
	default:
		return;
	}

	//ブレンディングをコンテキストを設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(setBlendState.Get(), blendFactor, 0xffffffff);
}

void DeviceManager::RenderEnd()
{
	pSwapChain->Present(0, 0);
}

DeviceManager& DeviceManager::GetInstance()
{
	static DeviceManager instance;
	return instance;
}