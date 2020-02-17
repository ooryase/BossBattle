#pragma once
#include"../System/Resource.h"
#include "../Polygon/Model/FbxModel.h"
#include "../Shader/ModelShader.h"
#include "../Polygon/ObjectManager.h"

struct Light
{
	DirectX::XMFLOAT4 Directional;
	DirectX::XMFLOAT4 Player;
	DirectX::XMFLOAT4 PColor;
	DirectX::XMFLOAT4 PAttenuation;
	DirectX::XMINT4 ELCount;
	DirectX::XMFLOAT4 Enemy[8];
	DirectX::XMFLOAT4 EColor[8];
	DirectX::XMFLOAT4 EAttenuation[8];
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