#pragma once
#include"BaseModel.h"

class PolyGon
{
protected:
	bool anim;
	std::string animName;

	ComPtr<ID3D11RasterizerState> pRasterizerState;

	ComPtr<ID3D11Buffer> pVerBuffer;
	ComPtr<ID3D11Buffer> pIndexBuffer;
	ComPtr<ID3D11Buffer> pLineAdjIndexBuffer;

	std::vector<MODEL::VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> lineAdj;
public:
	BaseModel();
	~BaseModel();

protected:
	void CreateBuffer(ComPtr<ID3D11Device> pDevice);
	virtual void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice) = 0;
	virtual void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice) = 0;
	virtual void CreateRasterizeState(ComPtr<ID3D11Device> pDevice) = 0;

public:
	virtual void Update(int speedDiv = 1) = 0;
	virtual void SetAnimSackNumber(int num) = 0;
	virtual void UpdateAnotherTimeCount(FbxTime& _timeCount) = 0;
	virtual void UpdateToAnotherTimeCount(int speedDiv, FbxTime& _timeCount) = 0;
	virtual void SetAnimSackNumberAnotherTimeCount(int num, FbxTime& _timeCount) = 0;

	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawLineAdjSet(ComPtr<ID3D11DeviceContext> pDeviceContext);

	int GetVertexCount() { return vertices.size(); }
	int GetIndexCount() { return indices.size(); }
	int GetLineAdjIndexCount() { return lineAdj.size(); }
};