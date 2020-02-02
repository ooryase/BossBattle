#pragma once
#include "../BaseObject.h"

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

class BaseCharacter : public BaseObject
{

protected:
	float hp;

	float e; //îΩî≠åWêî

	bool die;
	bool dead;

	//IsHit();

	std::shared_ptr<Param> param;


public:
	BaseCharacter(std::shared_ptr<Light> _light);
	~BaseCharacter() {};

	virtual void Update() = 0;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);
protected:
	//void CollisionNormal();
	//void CollisionStealth();
	//void CollisionDamage();

};