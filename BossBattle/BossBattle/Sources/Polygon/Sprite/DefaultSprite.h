#pragma once
#include"Sprite.h"

class DefaultSprite : public Sprite
{
public:
	DefaultSprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[]);

private:
	void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateRasterizerState(ComPtr<ID3D11Device> pDevice);
};