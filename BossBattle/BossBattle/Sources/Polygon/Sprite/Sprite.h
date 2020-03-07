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
protected:
	ComPtr<ID3D11Texture2D> pTexture;
	ComPtr<ID3D11ShaderResourceView> pSRV;
	ComPtr<ID3D11SamplerState> pSampler;

	ComPtr<ID3D11RasterizerState> pRasterizerState;

	ComPtr<ID3D11Buffer> pVerBuffer;
	ComPtr<ID3D11Buffer> pIndexBuffer;

	std::vector<SPRITE::VERTEX> vertices;
	std::vector<UINT> indices;

public:
	Sprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[],
		D3D11_FILTER _filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	~Sprite();

	int GetIndexCount() { return indices.size(); };
	virtual void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);

	virtual void Scroll(float param);
protected:
	void TextureLoad(ComPtr<ID3D11Device> pDevice, const wchar_t filename[]);
	virtual void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice) = 0;
	virtual void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice) = 0;
	virtual void CreateRasterizerState(ComPtr<ID3D11Device> pDevice) = 0;

	void CreateSamplar(ComPtr<ID3D11Device> pDevice, D3D11_FILTER _filter);
};