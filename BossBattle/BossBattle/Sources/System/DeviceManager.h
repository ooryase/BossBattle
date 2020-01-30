#pragma once
#include"Resource.h"

class DeviceManager
{
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pDeviceContext;
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11Texture2D> pDepthStencil;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	ComPtr<ID3D11BlendState> pBlendState;

	D3D11_VIEWPORT viewPort[1];

public:
	DeviceManager(HWND WHandle);
	~DeviceManager();

	HRESULT HResult;

	void RenderBegin();

	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return pDeviceContext; }
};