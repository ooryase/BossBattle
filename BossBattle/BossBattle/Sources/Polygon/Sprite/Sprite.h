#pragma once
#include "../../System/Resource.h"
#include <wincodec.h>

namespace SPRITE
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 TexUV;

		VERTEX(DirectX::XMFLOAT3 _Pos, DirectX::XMFLOAT2 _uv) :
			Pos(_Pos), TexUV(_uv) {}
	};
}

class Sprite
{
	ID3D11Texture2D* pTexture;
	ID3D11ShaderResourceView* pSRV;
	ID3D11SamplerState* pSampler;

	ID3D11RasterizerState* pRasterizerState;

	ID3D11Buffer* pVerBuffer;
	ID3D11Buffer* pIndexBuffer;

	std::vector<SPRITE::VERTEX> vertices;
	std::vector<UINT> indices;

public:
	Sprite(ID3D11Device* pDevice, const wchar_t filename[]);
	~Sprite();

	int GetIndexCount() { return indices.size(); };
	void DrawSet(ID3D11DeviceContext* pDeviceContext);
private:
	void TextureLoad(ID3D11Device* pDevice, const wchar_t filename[]);
	void CreateVertexBuffer(ID3D11Device* pDevice);
	void CreateIndexBuffer(ID3D11Device* pDevice);
	void CreateRasterizerState(ID3D11Device* pDevice);
};