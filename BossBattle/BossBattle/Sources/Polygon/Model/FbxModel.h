#pragma once;

#include"BaseModel.h"
#include"fbxsdk.h"
#include"../../System/Timer.h"

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
	FbxModel(ComPtr<ID3D11Device> pDevice, const char filename[]);
	~FbxModel();

	void FbxLoad();
	void AnimationLoad();

	void CreateVertexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateIndexBuffer(ComPtr<ID3D11Device> pDevice);
	void CreateRasterizeState(ComPtr<ID3D11Device> pDevice);

	void SetAnimSackNumber(int num);

	void Update(int speedDiv = 1);
};