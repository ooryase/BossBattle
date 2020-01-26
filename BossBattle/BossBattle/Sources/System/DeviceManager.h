#pragma once
#include"Resource.h"

class DeviceManager
{
	//template<typename T>
	//using ComP = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pDeviceContext;
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	ComPtr<ID3D11Texture2D> pDepthStencil;
	ComPtr<ID3D11DepthStencilView> pDepthStencilView;

	//IDXGISwapChain* pSwapChain;
	//ID3D11Device* pDevice;
	//ID3D11DeviceContext* pDeviceContext;
	//ID3D11RenderTargetView* pRenderTargetView;
	//ID3D11Texture2D* pDepthStencil;
	//ID3D11DepthStencilView* pDepthStencilView;
	D3D11_VIEWPORT viewPort[1];

public:
	DeviceManager(HWND WHandle);
	~DeviceManager();

	HRESULT HResult;

	void RenderBegin();

	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return pDeviceContext; }
	//ID3D11Device* GetDevice() { return pDevice; }
	//ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext; }
};