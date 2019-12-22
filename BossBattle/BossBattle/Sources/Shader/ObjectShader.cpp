#include "ObjectShader.h"

ObjectShader::ObjectShader(ID3D11Device* pDevice, const wchar_t fileName[])
{
	// シェーダの設定
	ID3DBlob* pCompileVS = NULL;
	ID3DBlob* pCompilePS = NULL;
	ID3DBlob* pCompileGS = NULL;
	D3DCompileFromFile( fileName, NULL, NULL, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	D3DCompileFromFile( fileName, NULL, NULL, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	D3DCompileFromFile( fileName, NULL, NULL, "GS", "gs_5_0", NULL, 0, &pCompileGS, NULL);
	pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
	pDevice->CreateGeometryShader(pCompileGS->GetBufferPointer(), pCompileGS->GetBufferSize(), NULL, &pGeometryShader);

	// 頂点レイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	pCompileVS->Release();
	pCompilePS->Release();
}

ObjectShader::~ObjectShader()
{
	SAFE_RELEASE(pVertexLayout);

	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pGeometryShader);
}