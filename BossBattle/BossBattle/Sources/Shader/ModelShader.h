#pragma once
#include"../System/Resource.h"

namespace MODEL
{
	struct CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT4 EdgeColor;
	};
}

class ModelShader
{
	ComPtr<ID3D11InputLayout> pVertexLayout;
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11GeometryShader> pGeometryShader;
	ComPtr<ID3D11PixelShader> pPixelShader;

	ComPtr<ID3D11InputLayout> pLineAdjVertexLayout;
	ComPtr<ID3D11VertexShader> pLineAdjVertexShader;
	ComPtr<ID3D11GeometryShader> pLineAdjGeometryShader;
	ComPtr<ID3D11PixelShader> pLineAdjPixelShader;

	ComPtr<ID3D11Buffer> pConstantBuffer;

public:
	ModelShader(ComPtr<ID3D11Device> pDevice, const wchar_t fileName[], const wchar_t lineAdjFileName[]);
	~ModelShader();

	void ShaderLoad(ComPtr<ID3D11Device> pDevice, const wchar_t fileName[],
		ComPtr<ID3D11InputLayout>* _pLayout,
		ComPtr<ID3D11VertexShader>* _pVertex,
		ComPtr<ID3D11GeometryShader>* _pGeometry,
		ComPtr<ID3D11PixelShader>* _pPixel);

	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawLineAdjSet(ComPtr<ID3D11DeviceContext> pDeviceContext);

	void SetConstantBuffer(ComPtr<ID3D11DeviceContext> pDeviceContext, MODEL::CONSTANT_BUFFER constantBuffer);
};