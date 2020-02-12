#pragma once
#include"Sprite.h"

class OctagonSprite : public Sprite
{
public:
	OctagonSprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[]);

	void Scroll(float param) override;

	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

private:
	void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateRasterizerState(ComPtr<ID3D11Device> pDevice);
};