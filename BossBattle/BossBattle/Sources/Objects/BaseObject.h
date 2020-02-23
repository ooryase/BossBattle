#pragma once
#include"../System/Resource.h"
#include "../Polygon/Model/FbxModel.h"
#include "../Shader/ModelShader.h"
#include "../Polygon/ObjectManager.h"

struct Light
{
	DirectX::XMFLOAT3 Directional;
	int PointCount;
	DirectX::XMFLOAT4 Pos[8];
	DirectX::XMFLOAT4 Color[8];
	DirectX::XMFLOAT4 Attenuation[8];

	bool SetPointLight(DirectX::XMFLOAT4 _pos,
		DirectX::XMFLOAT4 _col,
		DirectX::XMFLOAT4 _attenuation);
};

enum class ObjectTag
{
	NORMAL,
	STEALTH,
	DAMAGE,
	//ITEM,
	//EVENT,
};

struct DParam
{
	float damage;
	DirectX::XMFLOAT3 direction;
};



class BaseObject
{
protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMFLOAT3 rotateDef; //モデルを正面に向かせるためのデフォルト値

	ObjectTag tag;

	float radius; //当たり判定の半径

	bool isToDelete;

public:
	BaseObject();
	~BaseObject();

	virtual void Update() = 0;
	virtual void EndUpdate() = 0;
	virtual void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;

protected:
	virtual void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;

public:
	virtual void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta) = 0;

public:
	DirectX::XMFLOAT3 GetPos() const;
	float GetRadius() const;
	ObjectTag GetTag() const;
	void SetTag(ObjectTag _tag);

	bool IsDead() const;
};