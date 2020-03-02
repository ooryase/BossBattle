#include"DeviceManager.h"

HRESULT DeviceManager::Init(HWND WHandle)
{
	HRESULT hr = CoInitialize(NULL);

	hr = CreateDeviceAndSwapChain(WHandle);
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

	hr = CreateRenderTarget();
	if (FAILED(hr))
		return hr;

	CreateRadialBlur();

	radialBlur.Center = DirectX::XMFLOAT3(0.5f,0.5f, 0.3f);
	radialBlur.Value = 1.0f;

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
	CLIENT_WIDTH = rc.right - rc.left;
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

HRESULT DeviceManager::CreateRenderTarget()
{
	// 描画可能テクスチャの作成
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = CLIENT_WIDTH;	// 幅
	desc.Height = CLIENT_HEIGHT;	// 高さ
	desc.MipLevels = 1;		// ミップマップ レベル数
	desc.ArraySize = 1;		// 配列サイズ
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // フォーマット
	desc.SampleDesc.Count = 1;		// マルチサンプル数
	desc.Usage = D3D11_USAGE_DEFAULT; // 使用法
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = pDevice->CreateTexture2D(&desc, NULL, &pRTTexture);
	if (FAILED(hr))
		return hr;

	// 描画可能テクスチャの描画ターゲット ビューの作成
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = desc.Format; // テクスチャのフォーマットを指定
	rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(
		pRTTexture.Get(),      // ビューを作るテクスチャ リソース
		&rtDesc,           // ビューの設定
		pRTTextureRTV.GetAddressOf()); // ビューを受け取る変数
	if (FAILED(hr))
		return hr;

	// 描画可能テクスチャのシェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format; // テクスチャのフォーマットを指定
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(
		pRTTexture.Get(),       // ビューを作るテクスチャ リソース
		&srDesc,            // ビューの設定
		pRTTextureSRV.GetAddressOf());  // ビューを受け取る変数
	if (FAILED(hr))
		return hr;

	// ビューポートの設定
	vpRTTexture[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	vpRTTexture[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	vpRTTexture[0].Width = (FLOAT)desc.Width;	// ビューポート領域の幅
	vpRTTexture[0].Height = (FLOAT)desc.Height;// ビューポート領域の高さ
	vpRTTexture[0].MinDepth = 0.0f;	// ビューポート領域の深度値の最小値
	vpRTTexture[0].MaxDepth = 1.0f;	// ビューポート領域の深度値の最大値

	// 描画ターゲット用の深度/ステンシル テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = CLIENT_WIDTH;   // 幅
	descDepth.Height = CLIENT_HEIGHT;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;  // フォーマット
	descDepth.SampleDesc.Count = 1;  // マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 深度/ステンシルとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	hr = pDevice->CreateTexture2D(
		&descDepth,				// 作成する2Dテクスチャの設定
		NULL,					// 
		pRTDepthStencil.GetAddressOf());	// 作成したテクスチャを受け取る変数
	if (FAILED(hr))
		return hr;// DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);  // 失敗

	// 描画ターゲット用の深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(
		pRTDepthStencil.Get(),		// 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,				// 深度/ステンシル・ビューの設定
		pRTDepthStencilView.GetAddressOf());// 作成したビューを受け取る変数
	if (FAILED(hr))
		return hr;// DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilView", hr);  // 失敗



	{
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0, 1)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(-0.5f, 0.5f,  0.5f), DirectX::XMFLOAT2(0, 0)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(0.5f, -0.5f,  0.0f), DirectX::XMFLOAT2(1, 1)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(0.5f, 0.5f,   0.0f), DirectX::XMFLOAT2(1, 0)));

		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(VERTEX) * vertices.size();
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices.data();
		pDevice->CreateBuffer(&bd, &initData, pVerBuffer.GetAddressOf());
	}
	{
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(UINT) * indices.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = indices.data();
		pDevice->CreateBuffer(&bd, &initData, pIndexBuffer.GetAddressOf());
	}

	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0;
	rdc.SlopeScaledDepthBias = 0;
	rdc.DepthClipEnable = TRUE;
	rdc.ScissorEnable = FALSE;
	rdc.MultisampleEnable = FALSE;
	rdc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&rdc, pRTRasterizerState.GetAddressOf());


	return S_OK;
}

HRESULT DeviceManager::CreateRadialBlur()
{

	// シェーダの設定
	ComPtr<ID3DBlob> pCompileVS;
	ComPtr<ID3DBlob> pCompilePS;
	ComPtr<ID3DBlob> pCompileGS;
	D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "VS", "vs_5_0", NULL, 0, pCompileVS.GetAddressOf(), NULL);
	D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "PS", "ps_5_0", NULL, 0, pCompilePS.GetAddressOf(), NULL);
	D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "GS", "gs_5_0", NULL, 0, pCompileGS.GetAddressOf(), NULL);
	pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, pRTVertexShader.GetAddressOf());
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, pRTPixelShader.GetAddressOf());
	pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, pRTGeometryShader.GetAddressOf());

	// 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), pRTVertexLayout.GetAddressOf());
	pCompileVS.Reset();
	pCompilePS.Reset();
	pCompileGS.Reset();


	//サンプラーの設定
	D3D11_SAMPLER_DESC smpdesc;
	ZeroMemory(&smpdesc, sizeof(D3D11_SAMPLER_DESC));
	smpdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	pDevice->CreateSamplerState(&smpdesc, pRTTextureSampler.GetAddressOf());

	//// 定数バッファの設定
	D3D11_BUFFER_DESC cbuffer;
	cbuffer.ByteWidth = sizeof(RADIAL_BLUR);
	cbuffer.Usage = D3D11_USAGE_DYNAMIC;
	cbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbuffer.MiscFlags = 0;
	cbuffer.StructureByteStride = 0;
	HRESULT hr = pDevice->CreateBuffer(&cbuffer, NULL, pRadialBlurBuffer.GetAddressOf());
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void DeviceManager::RenderBegin()
{
	// 描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	//pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);
	//
	//// 深度/ステンシル値のクリア
	//pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	//
	//// ラスタライザにビューポートを設定
	//pDeviceContext->RSSetViewports(1, viewPort);
	//
	//// 描画ターゲット・ビューを出力マージャーの描画ターゲットとして設定
	//pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	//
	//
	////ブレンディングをコンテキストを設定
	//float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//pDeviceContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);


	pDeviceContext->ClearRenderTargetView(pRTTextureRTV.Get(), ClearColor);
	pDeviceContext->ClearDepthStencilView(pRTDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->RSSetViewports(1, vpRTTexture);
	// OMに描画ターゲット ビューを設定
	pDeviceContext->OMSetRenderTargets(1, pRTTextureRTV.GetAddressOf(), pRTDepthStencilView.Get());

	//float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//pDeviceContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);
	SetBerendState(BLEND_STATE::ALIGMENT);
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
	pDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	// 描画ターゲットのクリア
	float ClearColor[4] = { 0.5f, 0.125f, 0.3f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);
	
	// 深度/ステンシル値のクリア
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	// ラスタライザにビューポートを設定
	pDeviceContext->RSSetViewports(1, viewPort);
	
	// 描画ターゲット・ビューを出力マージャーの描画ターゲットとして設定
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	
	SetBerendState(BLEND_STATE::ALIGMENT);

	//ブレンディングをコンテキストを設定
	//float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//pDeviceContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVerBuffer.GetAddressOf(), &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetSamplers(0, 1, pRTTextureSampler.GetAddressOf());
	pDeviceContext->PSSetShaderResources(0, 1, pRTTextureSRV.GetAddressOf());
	pDeviceContext->RSSetState(pRTRasterizerState.Get());

	pDeviceContext->IASetInputLayout(pRTVertexLayout.Get());
	pDeviceContext->VSSetShader(pRTVertexShader.Get(), NULL, 0);
	pDeviceContext->GSSetShader(pRTGeometryShader.Get(), NULL, 0);
	pDeviceContext->PSSetShader(pRTPixelShader.Get(), NULL, 0);

	pDeviceContext->VSSetConstantBuffers(2, 1, pRadialBlurBuffer.GetAddressOf());
	pDeviceContext->PSSetConstantBuffers(2, 1, pRadialBlurBuffer.GetAddressOf());
	
	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(pRadialBlurBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&radialBlur), sizeof(radialBlur));
	pDeviceContext->Unmap(pRadialBlurBuffer.Get(), 0);


	pDeviceContext->Draw(vertices.size(), 0);

	pSwapChain->Present(0, 0);
}

DeviceManager& DeviceManager::GetInstance()
{
	static DeviceManager instance;
	return instance;
}