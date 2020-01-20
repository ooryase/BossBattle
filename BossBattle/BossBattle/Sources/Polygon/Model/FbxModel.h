#pragma once;

//#include"../../System/Resource.h"
#include"BaseModel.h"
#include"fbxsdk.h"

#pragma comment(lib, "libfbxsdk-md.lib")

class FbxModel : public BaseModel
{
	FbxManager* fbxManager;
	FbxScene* fbxScene;
	FbxNode* fbxMeshNode;
	FbxMesh* fbxMesh;

	FbxArray<FbxString*> AnimStackNameArray;
	int AnimStackNumber;
	FbxTime FrameTime, timeCount, start, stop;


public:
	FbxModel(ID3D11Device* pDevice, const char filename[]);
	~FbxModel();

	void FbxLoad();

	void CreateVertexBuffer(ID3D11Device* pDevice);
	void CreateIndexBuffer(ID3D11Device* pDevice);
	void CreateRasterizeState(ID3D11Device* pDevice);

	void SetAnimSackNumber(int num);

	void Update();
};