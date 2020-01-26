#pragma once
#include"../System/Resource.h"

namespace SPRITE
{
	struct CONSTANT_BUFFER {
		DirectX::XMFLOAT4X4 World;
	};
}

class SpriteShader
{
	ComPtr<ID3D11InputLayout> pVertexLayout;

	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11GeometryShader> pGeometryShader;
	ComPtr<ID3D11PixelShader> pPixelShader;

	ComPtr<ID3D11Buffer> pConstantBuffer;

public:
	SpriteShader(ComPtr<ID3D11Device> pDevice, const wchar_t fileName[]);
	~SpriteShader();

	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void SetConstantBuffer(ComPtr<ID3D11DeviceContext> pDeviceContext, SPRITE::CONSTANT_BUFFER constantBuffer);
};