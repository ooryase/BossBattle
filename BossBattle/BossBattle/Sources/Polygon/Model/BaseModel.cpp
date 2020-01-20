#include"BaseModel.h"

using namespace MODEL;

BaseModel::BaseModel()
{
	anim = true;
}

BaseModel::~BaseModel()
{
	SAFE_RELEASE(pVerBuffer);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pLineAdjIndexBuffer);
	SAFE_RELEASE(pRasterizerState);
}

void BaseModel::CreateBuffer(ID3D11Device* pDevice)
{
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateRasterizeState(pDevice);
}

void BaseModel::DrawSet(ID3D11DeviceContext* pDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE pdata;
	//頂点パラメータの受け渡し
	pDeviceContext->Map(pVerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertices.data()), sizeof(VERTEX) * vertices.size());
	pDeviceContext->Unmap(pVerBuffer, 0);


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVerBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//pDeviceContext->PSSetSamplers(0, 1, &pSampler);
	//pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	pDeviceContext->RSSetState(pRasterizerState);

	//頂点パラメータの受け渡し
	//D3D11_MAPPED_SUBRESOURCE pdata;
	//pDeviceContext->Map(pVerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertices.data()), sizeof(VERTEX) * vertices.size());
	//pDeviceContext->Unmap(pVerBuffer, 0);
}

void BaseModel::DrawLineAdjSet(ID3D11DeviceContext* pDeviceContext)
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVerBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
	pDeviceContext->IASetIndexBuffer(pLineAdjIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//pDeviceContext->PSSetSamplers(0, 1, &pSampler);
	//pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	pDeviceContext->RSSetState(pRasterizerState);

	//頂点パラメータの受け渡し
	//D3D11_MAPPED_SUBRESOURCE pdata;
	//pDeviceContext->Map(pVerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(vertices.data()), sizeof(VERTEX) * vertices.size());
	//pDeviceContext->Unmap(pVerBuffer, 0);

}