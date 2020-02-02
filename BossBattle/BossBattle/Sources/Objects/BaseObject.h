#pragma once
#include"../System/Resource.h"
#include "../Polygon/Model/FbxModel.h"
#include "../Shader/ModelShader.h"
#include "../Polygon/ObjectManager.h"

struct Light
{
	DirectX::XMFLOAT3 playerLight;
	DirectX::XMFLOAT4 playerColor;
	DirectX::XMFLOAT4 playerAttenuation;
	//DirectX::XMVECTOR

	Light() :
		playerLight(DirectX::XMFLOAT3(40.1f, -20.0f, -5.0f)),
		playerColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
		playerAttenuation(DirectX::XMFLOAT4(1.0f, 0.0005f, 0.0005f, 0.0f))
	{}
};

enum class ObjectTag
{
	NORMAL,
	STEALTH,
	DAMAGE,
	//ITEM,
	//EVENT,
};


class BaseObject
{
protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMFLOAT3 rotateDef; //モデルを正面に向かせるためのデフォルト値

	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> shader;

	std::shared_ptr<Light> light;

	ObjectTag tag;

	float radius; //当たり判定の半径

public:
	BaseObject(std::shared_ptr<Light> _light);
	~BaseObject();

	virtual void Update() = 0;
	virtual void EndUpdate() = 0;
	virtual void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);

protected:
	virtual void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);


public:
	virtual void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta) = 0;

public:
	DirectX::XMFLOAT3 GetPos() const;
	float GetRadius() const;
	ObjectTag GetTag() const;

};