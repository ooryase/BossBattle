#pragma once
#include"Resource.h"

class DeviceManager
{
	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11Texture2D* pDepthStencil;
	ID3D11DepthStencilView* pDepthStencilView;
	D3D11_VIEWPORT viewPort[1];

public:
	DeviceManager(HWND WHandle);
	~DeviceManager();

	HRESULT HResult;

	void RenderBegin();

	void RenderEnd();
};