#pragma once
#include "../BaseObject.h"
#include "../Effect/BaseEffect.h"

struct Param
{
	DirectX::XMFLOAT3 speed;
	DirectX::XMFLOAT3 direction;
	bool ground;
	float gravity;
	const float GRAVITY_DEF;

	Param(DirectX::XMFLOAT3 _speed, float gravityDefault) :
		speed(_speed),
		direction(DirectX::XMFLOAT3(0.0f,0.0f,DirectX::XM_PIDIV2)),
		ground(false),
		gravity(gravityDefault),
		GRAVITY_DEF(gravityDefault)
	{}

};

class BaseCharacter : public BaseObject, public std::enable_shared_from_this<BaseCharacter>
{

protected:
	float hp;
	float maxHp;

	float e; //�����W��

	bool isDead;

	//IsHit();

	std::shared_ptr<Light> light;

	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> shader;


	std::shared_ptr<Param> param;

	std::vector< std::shared_ptr< BaseEffect>> *effectReserves;
	std::shared_ptr<ObjectManager> objectManager;

public:
	BaseCharacter(std::shared_ptr<Light> _light,
		std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
		std::shared_ptr<ObjectManager> _objectManager);
	~BaseCharacter() {};

	virtual void Update() = 0;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

	void CollisionNormal(DirectX::XMFLOAT3 delta);
	void CollisionDamage(DParam* dParam);

protected:

public:
	virtual void AttackHit(int type, int quantity);

	virtual void SetObjectReserved(std::shared_ptr<BaseCharacter> _chara);
	virtual void SetEffectReserved(std::shared_ptr<BaseEffect> _obj);

	std::shared_ptr<ObjectManager> GetObjectManager() const;
	std::shared_ptr<Light> GetLight() const;
	DirectX::XMFLOAT3 GetDirection() const;
};