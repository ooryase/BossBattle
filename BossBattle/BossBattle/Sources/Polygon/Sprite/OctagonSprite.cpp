#include "OctagonSprite.h"

using SPRITE::VERTEX;
using namespace DirectX;

OctagonSprite::OctagonSprite(ComPtr<ID3D11Device> pDevice, const wchar_t filename[]) 
	: Sprite(pDevice, filename)
{
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateRasterizerState(pDevice);
}

void OctagonSprite::CreateVertexBuffer(ComPtr<ID3D11Device> pDevice)
{
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 0.10f,	 1.0f * 0.5f, 0.0f), XMFLOAT2(0, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(0.72f * 0.10f,	 0.72f * 0.5f, 0.0f), XMFLOAT2(0.125f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 1.0f * 0.10f,	 0.0f * 0.5f, 0.0f), XMFLOAT2(0.25f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(0.72f * 0.10f,	 -0.72f * 0.5f, 0.0f), XMFLOAT2(0.375f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 0.10f,	 -1.0f * 0.5f, 0.0f), XMFLOAT2(0.5f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 0.1f,	 -0.72f * 0.5f, 0.0f), XMFLOAT2(0.625f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-1.0f * 0.1f,	0.0f * 0.5f, 0.0f), XMFLOAT2(0.75f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 0.1f,	 0.72f * 0.5f, 0.0f), XMFLOAT2(0.875f, 1.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 0.1f,	 1.0f * 0.5f, 0.0f), XMFLOAT2(1.0f, 1.0f)));

	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 2.0f,	 1.0f * 2.0f,	 0.0f), XMFLOAT2(0, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3(0.72f * 2.0f,	 0.72f * 2.0f,	 0.0f), XMFLOAT2(0.125f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3( 1.0f * 2.0f,	 0.0f * 2.0f,	 0.0f), XMFLOAT2(0.25f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3(0.72f * 2.0f,	 -0.72f * 2.0f,	 0.0f), XMFLOAT2(0.375f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 2.0f,	 -1.0f * 2.0f,	 0.0f), XMFLOAT2(0.5f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 2.0f,	 -0.72f * 2.0f,	 0.0f), XMFLOAT2(0.625f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3( -1.0f * 2.0f,	 0.0f * 2.0f,	 0.0f), XMFLOAT2(0.75f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 2.0f,	 0.72f * 2.0f,	 0.0f), XMFLOAT2(0.875f, 0.5f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 2.0f,	 1.0f * 2.0f,	 0.0f), XMFLOAT2(1.0f, 0.5f)));

	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 10.0f,	 1.0f * 10.0f,	 -5.0f), XMFLOAT2(0, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(0.72f * 10.0f,	 0.72f * 10.0f,	 -5.0f), XMFLOAT2(0.125f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 1.0f * 10.0f,	 0.0f * 10.0f,	 -5.0f), XMFLOAT2(0.25f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.72f * 10.0f,	 -0.72f * 10.0f,	 -5.0f), XMFLOAT2(0.375f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 10.0f,	 -1.0f * 10.0f,	 -5.0f), XMFLOAT2(0.5f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 10.0f,	 -0.72f * 10.0f,	 -5.0f), XMFLOAT2(0.625f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-1.0f * 10.0f,	 0.0f * 10.0f,	 -5.0f), XMFLOAT2(0.75f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3(-0.72f * 10.0f,	 0.72f * 10.0f,	 -5.0f), XMFLOAT2(0.875f, 0.0f)));
	vertices.push_back(VERTEX(XMFLOAT3( 0.0f * 10.0f,	 1.0f * 10.0f,	 -5.0f), XMFLOAT2(1.0f, 0.0f)));

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	pDevice->CreateBuffer(&bd, &initData, pVerBuffer.GetAddressOf());

}

void OctagonSprite::CreateIndexBuffer(ComPtr<ID3D11Device> pDevice)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			indices.push_back(0 + j + i * 9);
			indices.push_back(1 + j + i * 9);
			indices.push_back(9 + j + i * 9);

			indices.push_back(1 + j + i * 9);
			indices.push_back(10 + j + i * 9);
			indices.push_back(9 + j + i * 9);
		}
	}

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

void OctagonSprite::CreateRasterizerState(ComPtr<ID3D11Device> pDevice)
{
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_FRONT;
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

void OctagonSprite::Scroll(float param)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			vertices.at(i * 9 + j).TexUV.y = 0.5f - i * 0.25f + param * 0.5f;
		}
	}

}

void OctagonSprite::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE pdata;
	//頂点パラメータの受け渡し
	pDeviceContext->Map(pVerBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertices.data()), sizeof(VERTEX) * vertices.size());
	pDeviceContext->Unmap(pVerBuffer.Get(), 0);


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVerBuffer.GetAddressOf(), &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	pDeviceContext->PSSetShaderResources(0, 1, pSRV.GetAddressOf());
	pDeviceContext->RSSetState(pRasterizerState.Get());
}