#include "SpriteShader.h"

SpriteShader::SpriteShader(ComPtr<ID3D11Device> pDevice, const wchar_t fileName[])
{
	// シェーダの設定
	ComPtr<ID3DBlob> pCompileVS;
	ComPtr<ID3DBlob> pCompilePS;
	ComPtr<ID3DBlob> pCompileGS;
	D3DCompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", NULL, 0, pCompileVS.GetAddressOf(), NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", NULL, 0, pCompilePS.GetAddressOf(), NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "GS", "gs_5_0", NULL, 0, pCompileGS.GetAddressOf(), NULL);
	pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, pGeometryShader.GetAddressOf());

	// 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), pVertexLayout.GetAddressOf());
	pCompileVS.Reset();
	pCompilePS.Reset();
	pCompileGS.Reset();

	// 定数バッファの設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(SPRITE::CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	pDevice->CreateBuffer(&cb, NULL, pConstantBuffer.GetAddressOf());
}

SpriteShader::~SpriteShader()
{
	//SAFE_RELEASE(pVertexLayout);
	//
	//SAFE_RELEASE(pVertexShader);
	//SAFE_RELEASE(pPixelShader);
	//SAFE_RELEASE(pGeometryShader);
	//
	//SAFE_RELEASE(pConstantBuffer);
}

void SpriteShader::DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->IASetInputLayout(pVertexLayout.Get());
	pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	pDeviceContext->GSSetShader(pGeometryShader.Get(), NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);
}

void SpriteShader::SetConstantBuffer(ComPtr<ID3D11DeviceContext> pDeviceContext, SPRITE::CONSTANT_BUFFER constantBuffer)
{
	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer));
	pDeviceContext->Unmap(pConstantBuffer.Get(), 0);

	pDeviceContext->VSSetConstantBuffers(2, 1, pConstantBuffer.GetAddressOf());
}