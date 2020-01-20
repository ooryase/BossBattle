#include "SpriteShader.h"

SpriteShader::SpriteShader(ID3D11Device* pDevice, const wchar_t fileName[])
{
	// シェーダの設定
	ID3DBlob* pCompileVS = NULL;
	ID3DBlob* pCompilePS = NULL;
	ID3DBlob* pCompileGS = NULL;
	D3DCompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "GS", "gs_5_0", NULL, 0, &pCompileGS, NULL);
	pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
	pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, &pGeometryShader);

	// 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	pCompileVS->Release();
	pCompilePS->Release();

	// 定数バッファの設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(SPRITE::CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);
}

SpriteShader::~SpriteShader()
{
	SAFE_RELEASE(pVertexLayout);

	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pGeometryShader);

	SAFE_RELEASE(pConstantBuffer);
}

void SpriteShader::DrawSet(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->IASetInputLayout(pVertexLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->GSSetShader(pGeometryShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
}

void SpriteShader::SetConstantBuffer(ID3D11DeviceContext* pDeviceContext, SPRITE::CONSTANT_BUFFER constantBuffer)
{
	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer));
	pDeviceContext->Unmap(pConstantBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer);
}