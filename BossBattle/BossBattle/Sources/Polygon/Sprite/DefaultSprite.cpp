#include "DefaultSprite.h"

using SPRITE::VERTEX;
using namespace DirectX;

DefaultSprite::DefaultSprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[])
	: Sprite(pDevice, filename)
{
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateRasterizerState(pDevice);
}

void DefaultSprite::CreateVertexBuffer(ComPtr<ID3D11Device> pDevice)
{
	vertices.push_back(VERTEX(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT2(0, 1)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.5f, 0.5f, 0.0f), XMFLOAT2(0, 0)));
	vertices.push_back(VERTEX(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT2(1, 1)));
	vertices.push_back(VERTEX(XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT2(1, 0)));

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	pDevice->CreateBuffer(&bd, &initData, pVerBuffer.GetAddressOf());
}

void DefaultSprite::CreateIndexBuffer(ComPtr<ID3D11Device> pDevice)
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(UINT) * indices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices.data();
	pDevice->CreateBuffer(&bd, &initData, pIndexBuffer.GetAddressOf());
}

void DefaultSprite::CreateRasterizerState(ComPtr<ID3D11Device> pDevice)
{
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0;
	rdc.SlopeScaledDepthBias = 0;
	rdc.DepthClipEnable = TRUE;
	rdc.ScissorEnable = FALSE;
	rdc.MultisampleEnable = FALSE;
	rdc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&rdc, pRasterizerState.GetAddressOf());
}