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
	ID3D11InputLayout* pVertexLayout;

	ID3D11VertexShader* pVertexShader;
	ID3D11GeometryShader* pGeometryShader;
	ID3D11PixelShader* pPixelShader;

	ID3D11Buffer* pConstantBuffer;

public:
	SpriteShader(ID3D11Device* pDevice, const wchar_t fileName[]);
	~SpriteShader();

	void DrawSet(ID3D11DeviceContext* pDeviceContext);
	void SetConstantBuffer(ID3D11DeviceContext* pDeviceContext, SPRITE::CONSTANT_BUFFER constantBuffer);
};