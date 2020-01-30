#include"DeviceManager.h"

DeviceManager::DeviceManager(HWND WHandle)
{
	CoInitialize(NULL);

	//�E�B���h�E�̃N���C�A���g�G���A
	RECT rc;
	GetClientRect(WHandle, &rc);
	UINT CWIDTH;
	CWIDTH = rc.right - rc.left;
	UINT CHEIGHT;
	CHEIGHT = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC dscd;
	ZeroMemory(&dscd, sizeof(dscd));    // �\���̂̒l��������(�K�v�ȏꍇ)
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
	//HResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &dscd, &pSwapChain, &pDevice, NULL, &pDeviceContext);
	HResult = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &dscd, pSwapChain.GetAddressOf(), pDevice.GetAddressOf(), NULL, pDeviceContext.GetAddressOf());
	if (FAILED(HResult)) return;

	//�o�b�N�o�b�t�@�̏�����
	ComPtr<ID3D11Texture2D> pBackBuffer;
	HResult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
	if (FAILED(HResult)) return;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);
	HResult = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.GetAddressOf());
	pBackBuffer.Reset();
	if (FAILED(HResult)) return;

	//�[�x�A�X�e���V���@�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HResult = pDevice->CreateTexture2D(&descDepth, NULL, pDepthStencil.GetAddressOf());
	if (FAILED(HResult)) return;

	//�[�x�A�X�e���V���@�r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	HResult = pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDepthStencilView.GetAddressOf());
	if (FAILED(HResult)) return;

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
	pDevice->CreateBlendState(&descBS, pBlendState.GetAddressOf());


	//�r���[�|�[�g�̐ݒ�
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].Width = (float)CWIDTH;
	viewPort[0].Height = (float)CHEIGHT;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;
}

DeviceManager::~DeviceManager()
{
	//SAFE_RELEASE(pDepthStencilView);
	//SAFE_RELEASE(pDepthStencil);
	//SAFE_RELEASE(pRenderTargetView);
	//SAFE_RELEASE(pSwapChain);
	//SAFE_RELEASE(pDeviceContext);
	//SAFE_RELEASE(pDevice);

	CoUninitialize();
}

void DeviceManager::RenderBegin()
{
	// �`��^�[�Q�b�g�̃N���A
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);

	// �[�x/�X�e���V���l�̃N���A
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ���X�^���C�U�Ƀr���[�|�[�g��ݒ�
	pDeviceContext->RSSetViewports(1, viewPort);

	// �`��^�[�Q�b�g�E�r���[���o�̓}�[�W���[�̕`��^�[�Q�b�g�Ƃ��Đݒ�
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());


	//�u�����f�B���O���R���e�L�X�g��ݒ�
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);

}

void DeviceManager::RenderEnd()
{
	pSwapChain->Present(0, 0);
}
