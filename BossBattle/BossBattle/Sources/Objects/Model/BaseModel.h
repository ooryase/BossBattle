#pragma once
#include"../../System/Resource.h"

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Nor;

	VERTEX(DirectX::XMFLOAT3 _Pos, DirectX::XMFLOAT3 _Nor) :
		Pos(_Pos), Nor(_Nor) {}
};

class BaseModel
{
protected:
	bool anim;
	std::string animName;

	ID3D11Buffer* verBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* lineAdjIndexBuffer;
	//FbxArray<FbxString*> AnimStackNameArray;
	//int AnimStackNumber;
	//FbxTime FrameTime, timeCount, start, stop;

	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> lineAdj;

	BaseModel(){}
	~BaseModel();

protected:
	void BufferCreate();
};