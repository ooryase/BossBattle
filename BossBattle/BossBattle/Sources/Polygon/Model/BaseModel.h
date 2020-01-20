#pragma once
#include"../../System/Resource.h"

namespace MODEL
{
	struct VERTEX
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Nor;

		VERTEX(DirectX::XMFLOAT3 _Pos, DirectX::XMFLOAT3 _Nor) :
			Pos(_Pos), Nor(_Nor) {}
	};
}
class BaseModel
{
protected:
	bool anim;
	std::string animName;

	ID3D11RasterizerState* pRasterizerState;

	ID3D11Buffer* pVerBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pLineAdjIndexBuffer;

	std::vector<MODEL::VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> lineAdj;
public:
	BaseModel();
	~BaseModel();

protected:
	void CreateBuffer(ID3D11Device* pDevice);
	virtual void CreateVertexBuffer(ID3D11Device* pDevice) = 0;
	virtual void CreateIndexBuffer(ID3D11Device* pDevice) = 0;
	virtual void CreateRasterizeState(ID3D11Device* pDevice) = 0;

public:
	virtual void Update() = 0;
	virtual void SetAnimSackNumber(int num) = 0;

	void DrawSet(ID3D11DeviceContext* pDeviceContext);
	void DrawLineAdjSet(ID3D11DeviceContext* pDeviceContext);

	int GetVertexCount() { return vertices.size(); }
	int GetIndexCount() { return indices.size(); }
	int GetLineAdjIndexCount() { return lineAdj.size(); }
};