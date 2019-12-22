#include"BaseModel.h"

BaseModel::~BaseModel()
{
	SAFE_RELEASE(verBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(lineAdjIndexBuffer);
}

void BaseModel::BufferCreate()
{
	/*

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * fbxMesh->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DYNAMIC;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices.data();
	pDevice->CreateBuffer(&bd_vertex, &data_vertex, &verBuffer);


	// インデックスデータの取り出しとバッファの設定
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(UINT) * indices.size(); //sizeof(int) * fbxMesh->GetPolygonVertexCount() * 2;
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	bd_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = indices.data();
	pDevice->CreateBuffer(&bd_index, &data_index, &indexBuffer);

	// インデックスデータの取り出しとバッファの設定
	D3D11_BUFFER_DESC bd_index;
	bd_index.ByteWidth = sizeof(UINT) * lineAdj.size(); //sizeof(int) * fbxMesh->GetPolygonVertexCount() * 2;
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;
	bd_index.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = lineAdj.data();
	pDevice->CreateBuffer(&bd_index, &data_index, &indexBuffer);

	// ラスタライザの設定
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FrontCounterClockwise = TRUE;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0;
	rdc.SlopeScaledDepthBias = 0;
	rdc.DepthClipEnable = TRUE;
	rdc.ScissorEnable = FALSE;
	rdc.MultisampleEnable = FALSE;
	rdc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&rdc, &pRasterizerState);

	*/

}