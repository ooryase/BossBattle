#pragma once
#include"Resource.h"

enum class BLEND_STATE
{
	ALIGMENT,
	ADD,
};

class DeviceManager
{
	DeviceManager(const DeviceManager&) = delete;
	DeviceManager operator = (const DeviceManager&) = delete;
	DeviceManager() {};

	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pDeviceContext;
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11Texture2D> pDepthStencil;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	ComPtr<ID3D11BlendState> pBlendState;
	ComPtr<ID3D11BlendState> pAddBlendState;

	D3D11_VIEWPORT viewPort[1];

public:
	HRESULT Init(HWND WHandle);
	void Release();

	HRESULT CreateDeviceAndSwapChain(HWND WHandle);
	HRESULT CreateBackBuffer();
	HRESULT CreateBlendState();
	HRESULT CreateAddBlendState();

	void RenderBegin();

	void SetBerendState(BLEND_STATE blendState);

	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return pDeviceContext; }

	static DeviceManager& GetInstance();
};