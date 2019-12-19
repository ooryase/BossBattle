#pragma once;

#include"../../System/Resource.h"
#include"fbxsdk.h"

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Nor;
};

class FbxModel
{
	//ID3D11RasterizerState* pRasterizerState;
	//ID3D11VertexShader* pVertexShader;
	//ID3D11InputLayout* pVertexLayout;
	//ID3D11GeometryShader* pGeometryShader;
	//ID3D11PixelShader* pPixelShader;
	//ID3D11Buffer* pConstantBuffer;

	FbxManager* fbxManager;
	FbxScene* fbxScene;
	FbxNode* fbxMeshNode;
	FbxMesh* fbxMesh;
	//ID3D11Buffer* verBuffer;
	//ID3D11Buffer* indexBuffer;
	FbxArray<FbxString*> AnimStackNameArray;
	int AnimStackNumber;
	FbxTime FrameTime, timeCount, start, stop;

	std::vector<VERTEX> vertices;
	std::vector<UINT> lineAdj;

public:
	FbxModel(const char filename[]);
	~FbxModel();
};