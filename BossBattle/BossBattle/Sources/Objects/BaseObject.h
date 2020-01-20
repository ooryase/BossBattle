#pragma once
#include"../System/Resource.h"
#include "../Polygon/Model/FbxModel.h"
#include "../Shader/ModelShader.h"
#include "../Polygon/ObjectManager.h"

class BaseObject
{
protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMFLOAT3 rotateDef; //モデルを正面に向かせるためのデフォルト値

	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> shader;

public:
	BaseObject();
	~BaseObject();

	virtual void Update() = 0;
	virtual void EndUpdate() = 0;
	virtual void Draw(ID3D11DeviceContext* pDeviceContext);

protected:
	virtual void DrawSet(ID3D11DeviceContext* pDeviceContext);
};