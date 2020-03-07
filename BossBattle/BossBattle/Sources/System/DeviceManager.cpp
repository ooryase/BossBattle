#include"DeviceManager.h"
#include"Timer.h"

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

	radialBlur.Center = DirectX::XMFLOAT2(0.5f,0.5f);
	radialBlur.Value = DirectX::XMFLOAT2(0.0f, 0.0f);
	isRadialBlur = false;
	radialBlurMaxTime = 0;
	radialBlurTime = 0;

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
	//�E�B���h�E�̃N���C�A���g�G���A
	RECT rc;
	GetClientRect(WHandle, &rc);
	CLIENT_WIDTH = rc.right - rc.left;
	CLIENT_HEIGHT = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC dscd;
	ZeroMemory(&dscd, sizeof(dscd));    // �\���̂̒l��������(�K�v�ȏꍇ)
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


	//�r���[�|�[�g�̐ݒ�
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
	//�o�b�N�o�b�t�@�̏�����
	ComPtr<ID3D11Texture2D> pBackBuffer;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.GetAddressOf());
	pBackBuffer.Reset();
	if (FAILED(hr)) return hr;

	//�[�x�A�X�e���V���@�e�N�X�`���̍쐬
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

	//�[�x�A�X�e���V���@�r���[�̍쐬
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
	// �`��\�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = CLIENT_WIDTH;	// ��
	desc.Height = CLIENT_HEIGHT;	// ����
	desc.MipLevels = 1;		// �~�b�v�}�b�v ���x����
	desc.ArraySize = 1;		// �z��T�C�Y
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // �t�H�[�}�b�g
	desc.SampleDesc.Count = 1;		// �}���`�T���v����
	desc.Usage = D3D11_USAGE_DEFAULT; // �g�p�@
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = pDevice->CreateTexture2D(&desc, NULL, &pRTTexture);
	if (FAILED(hr))
		return hr;

	// �`��\�e�N�X�`���̕`��^�[�Q�b�g �r���[�̍쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = desc.Format; // �e�N�X�`���̃t�H�[�}�b�g���w��
	rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(
		pRTTexture.Get(),      // �r���[�����e�N�X�`�� ���\�[�X
		&rtDesc,           // �r���[�̐ݒ�
		pRTTextureRTV.GetAddressOf()); // �r���[���󂯎��ϐ�
	if (FAILED(hr))
		return hr;

	// �`��\�e�N�X�`���̃V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format; // �e�N�X�`���̃t�H�[�}�b�g���w��
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(
		pRTTexture.Get(),       // �r���[�����e�N�X�`�� ���\�[�X
		&srDesc,            // �r���[�̐ݒ�
		pRTTextureSRV.GetAddressOf());  // �r���[���󂯎��ϐ�
	if (FAILED(hr))
		return hr;

	// �r���[�|�[�g�̐ݒ�
	vpRTTexture[0].TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	vpRTTexture[0].TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	vpRTTexture[0].Width = (FLOAT)desc.Width;	// �r���[�|�[�g�̈�̕�
	vpRTTexture[0].Height = (FLOAT)desc.Height;// �r���[�|�[�g�̈�̍���
	vpRTTexture[0].MinDepth = 0.0f;	// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	vpRTTexture[0].MaxDepth = 1.0f;	// �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	// �`��^�[�Q�b�g�p�̐[�x/�X�e���V�� �e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = CLIENT_WIDTH;   // ��
	descDepth.Height = CLIENT_HEIGHT;  // ����
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v ���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;  // �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �[�x/�X�e���V���Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
	hr = pDevice->CreateTexture2D(
		&descDepth,				// �쐬����2D�e�N�X�`���̐ݒ�
		NULL,					// 
		pRTDepthStencil.GetAddressOf());	// �쐬�����e�N�X�`�����󂯎��ϐ�
	if (FAILED(hr))
		return hr;// DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateTexture2D", hr);  // ���s

	// �`��^�[�Q�b�g�p�̐[�x/�X�e���V�� �r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(
		pRTDepthStencil.Get(),		// �[�x/�X�e���V���E�r���[�����e�N�X�`��
		&descDSV,				// �[�x/�X�e���V���E�r���[�̐ݒ�
		pRTDepthStencilView.GetAddressOf());// �쐬�����r���[���󂯎��ϐ�
	if (FAILED(hr))
		return hr;// DXTRACE_ERR(L"InitDirect3D g_pD3DDevice->CreateDepthStencilView", hr);  // ���s


	return S_OK;
}

HRESULT DeviceManager::CreateRadialBlur()
{
	HRESULT hr;

	// �V�F�[�_�̐ݒ�
	ComPtr<ID3DBlob> pCompileVS;
	ComPtr<ID3DBlob> pCompilePS;
	ComPtr<ID3DBlob> pCompileGS;
	hr = D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "VS", "vs_5_0", NULL, 0, pCompileVS.GetAddressOf(), NULL);
	if (FAILED(hr))	return hr;
	hr = D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "PS", "ps_5_0", NULL, 0, pCompilePS.GetAddressOf(), NULL);
	if (FAILED(hr))	return hr;
	hr = D3DCompileFromFile(L"Shader/Sprite/RadialBlur.hlsl", NULL, NULL, "GS", "gs_5_0", NULL, 0, pCompileGS.GetAddressOf(), NULL);
	if (FAILED(hr))	return hr;
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, pRTVertexShader.GetAddressOf());
	if (FAILED(hr))	return hr;
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, pRTPixelShader.GetAddressOf());
	if (FAILED(hr))	return hr;
	hr = pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, pRTGeometryShader.GetAddressOf());
	if (FAILED(hr))	return hr;

	// ���_���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), pRTVertexLayout.GetAddressOf());
	if (FAILED(hr))	return hr;
	pCompileVS.Reset();
	pCompilePS.Reset();
	pCompileGS.Reset();


	//�T���v���[�̐ݒ�
	D3D11_SAMPLER_DESC smpdesc;
	ZeroMemory(&smpdesc, sizeof(D3D11_SAMPLER_DESC));
	smpdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = pDevice->CreateSamplerState(&smpdesc, pRTTextureSampler.GetAddressOf());
	if (FAILED(hr))	return hr;

	//// �萔�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC cbuffer;
	cbuffer.ByteWidth = sizeof(RADIAL_BLUR);
	cbuffer.Usage = D3D11_USAGE_DYNAMIC;
	cbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbuffer.MiscFlags = 0;
	cbuffer.StructureByteStride = 0;
	hr = pDevice->CreateBuffer(&cbuffer, NULL, pRadialBlurBuffer.GetAddressOf());
	if (FAILED(hr))	return hr;


	{
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.9f), DirectX::XMFLOAT2(0, 1)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(-1.0f, 1.0f, 0.9f), DirectX::XMFLOAT2(0, 0)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(1.0f, -1.0f, 0.9f), DirectX::XMFLOAT2(1, 1)));
		vertices.push_back(VERTEX(DirectX::XMFLOAT3(1.0f, 1.0f, 0.9f), DirectX::XMFLOAT2(1, 0)));

		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(VERTEX) * vertices.size();
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices.data();
		hr = pDevice->CreateBuffer(&bd, &initData, pVerBuffer.GetAddressOf());
		if (FAILED(hr))	return hr;
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
		hr = pDevice->CreateBuffer(&bd, &initData, pIndexBuffer.GetAddressOf());
		if (FAILED(hr))	return hr;
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
	hr = pDevice->CreateRasterizerState(&rdc, pRTRasterizerState.GetAddressOf());
	if (FAILED(hr))	return hr;

	return S_OK;
}

void DeviceManager::RenderBegin()
{
	// �`��^�[�Q�b�g�̃N���A
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	pDeviceContext->ClearRenderTargetView(pRTTextureRTV.Get(), ClearColor);
	pDeviceContext->ClearDepthStencilView(pRTDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->RSSetViewports(1, vpRTTexture);
	// OM�ɕ`��^�[�Q�b�g �r���[��ݒ�
	pDeviceContext->OMSetRenderTargets(1, pRTTextureRTV.GetAddressOf(), pRTDepthStencilView.Get());

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

	//�u�����f�B���O���R���e�L�X�g��ݒ�
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(setBlendState.Get(), blendFactor, 0xffffffff);
}

void DeviceManager::RenderEnd()
{
	pSwapChain->Present(0, 0);
}

void DeviceManager::RenderToBackBuffer()
{
	//�`��^�[�Q�b�g�̐؂�ւ�
	pDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	float ClearColor[4] = { 0.5f, 0.125f, 0.3f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->RSSetViewports(1, viewPort);
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	SetBerendState(BLEND_STATE::ALIGMENT);

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

	pDeviceContext->PSSetConstantBuffers(2, 1, pRadialBlurBuffer.GetAddressOf());

	UpdateRadialBlur();

	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(pRadialBlurBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&radialBlur), sizeof(radialBlur));
	pDeviceContext->Unmap(pRadialBlurBuffer.Get(), 0);

	//�`��
	pDeviceContext->Draw(vertices.size(), 0);

}

void DeviceManager::UpdateRadialBlur()
{
	if (!isRadialBlur)
		return;

	radialBlurTime += Timer::GetInstance().GetDeltaTime();

	auto value = radialBlurMaxTime / 2 - std::abs(radialBlurMaxTime / 2 - radialBlurTime);

	radialBlur.Value.x = radialBlurStrenght * ( (value > 300) ? 1.0f: value / 300.0f);

	if (radialBlurTime > radialBlurMaxTime)
	{
		isRadialBlur = false;
		radialBlur.Value.x = 0.0f;
	}
}


void DeviceManager::SetRadialBlur(DirectX::XMFLOAT2 pos, int time, float strenght)
{
	isRadialBlur = true;
	radialBlurMaxTime = time;
	radialBlurTime = 0;
	radialBlur.Center = pos;
	radialBlurStrenght = strenght;
}

DeviceManager& DeviceManager::GetInstance()
{
	static DeviceManager instance;
	return instance;
}