#pragma once
#include"Sprite.h"

class DefaultSprite : public Sprite
{
public:
	DefaultSprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[],
		D3D11_FILTER _filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	void Scroll(float param) override;
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

private:
	void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateRasterizerState(ComPtr<ID3D11Device> pDevice);
};