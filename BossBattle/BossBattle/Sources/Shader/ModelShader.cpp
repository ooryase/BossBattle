#include "ModelShader.h"

ModelShader::ModelShader(ID3D11Device* pDevice, const wchar_t fileName[], const wchar_t lineAdjFileName[])
{
	ShaderLoad(pDevice, fileName, &pVertexLayout, &pVertexShader, &pGeometryShader, &pPixelShader);
	ShaderLoad(pDevice, lineAdjFileName, &pLineAdjVertexLayout, &pLineAdjVertexShader, &pLineAdjGeometryShader, &pLineAdjPixelShader);
}

ModelShader::~ModelShader()
{
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pGeometryShader);
	SAFE_RELEASE(pPixelShader);

	SAFE_RELEASE(pLineAdjVertexLayout);
	SAFE_RELEASE(pLineAdjVertexShader);
	SAFE_RELEASE(pLineAdjGeometryShader);
	SAFE_RELEASE(pLineAdjPixelShader);

	SAFE_RELEASE(pConstantBuffer);
}

void ModelShader::ShaderLoad(ID3D11Device* pDevice, const wchar_t fileName[],
	ID3D11InputLayout** _pLayout,
	ID3D11VertexShader** _pVertex,
	ID3D11GeometryShader** _pGeometry,
	ID3D11PixelShader** _pPixel)
{
	// シェーダの設定
	ID3DBlob* pCompileVS = NULL;
	ID3DBlob* pCompilePS = NULL;
	ID3DBlob* pCompileGS = NULL;
	D3DCompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "GS", "gs_5_0", NULL, 0, &pCompileGS, NULL);
	D3DCompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, _pVertex);
	pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, _pGeometry);
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, _pPixel);

	// 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), _pLayout);
	pCompileVS->Release();
	pCompilePS->Release();

	// 定数バッファの設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(MODEL::CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

}

void ModelShader::DrawSet(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->IASetInputLayout(pVertexLayout);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->GSSetShader(pGeometryShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
}

void ModelShader::DrawLineAdjSet(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->IASetInputLayout(pLineAdjVertexLayout);
	pDeviceContext->VSSetShader(pLineAdjVertexShader, NULL, 0);
	pDeviceContext->GSSetShader(pLineAdjGeometryShader, NULL, 0);
	pDeviceContext->PSSetShader(pLineAdjPixelShader, NULL, 0);
}

void ModelShader::SetConstantBuffer(ID3D11DeviceContext* pDeviceContext, MODEL::CONSTANT_BUFFER constantBuffer)
{
	D3D11_MAPPED_SUBRESOURCE data;
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, data.RowPitch, (void*)(&constantBuffer), sizeof(constantBuffer));
	pDeviceContext->Unmap(pConstantBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer);
	pDeviceContext->GSSetConstantBuffers(1, 1, &pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(1, 1, &pConstantBuffer);
}