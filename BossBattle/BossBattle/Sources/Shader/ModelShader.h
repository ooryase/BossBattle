#pragma once
#include"../System/Resource.h"

namespace MODEL
{
	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4 Light;
		DirectX::XMFLOAT4 Attenuation;
	};
}

class ModelShader
{
	ID3D11InputLayout* pVertexLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11GeometryShader* pGeometryShader;
	ID3D11PixelShader* pPixelShader;

	ID3D11InputLayout* pLineAdjVertexLayout;
	ID3D11VertexShader* pLineAdjVertexShader;
	ID3D11GeometryShader* pLineAdjGeometryShader;
	ID3D11PixelShader* pLineAdjPixelShader;

	ID3D11Buffer* pConstantBuffer;

public:
	ModelShader(ID3D11Device* pDevice, const wchar_t fileName[], const wchar_t lineAdjFileName[]);
	~ModelShader();

	void ShaderLoad(ID3D11Device* pDevice, const wchar_t fileName[],
		ID3D11InputLayout** _pLayout,
		ID3D11VertexShader** _pVertex,
		ID3D11GeometryShader** _pGeometry,
		ID3D11PixelShader** _pPixel);

	void DrawSet(ID3D11DeviceContext* pDeviceContext);
	void DrawLineAdjSet(ID3D11DeviceContext* pDeviceContext);

	void SetConstantBuffer(ID3D11DeviceContext* pDeviceContext, MODEL::CONSTANT_BUFFER constantBuffer);
};