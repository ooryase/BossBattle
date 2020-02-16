#pragma once
#include"../BaseCharacter.h"
#include"../../../System/Timer.h"
#include"../../../System/InputController.h"

namespace GUN_BEHAVE
{
	enum BehaveName
	{
		NONE = -1,
		WAIT,
		DASH,
		RUN,
		JUMP,
		FALL,
		DAMEGED,
		RECOVER,
		STEP,
		BREAD1,
		BREAD2,
		BREAD3,
		GUN1,
		GUN2,
		GUN3,
		SHIFT_BREAD1,
		SHIFT_BREAD2,
		SHIFT_GUN1,
		SHIFT_GUN2,
		SPECIAL,
	};

	enum class AttackType
	{
		NONE = -1,
		GUN,
		BREAD,
	};

}

struct GunBehave
{
	GunBehave(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player);
	~GunBehave();

	std::shared_ptr<Param> param;

	virtual void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light) = 0;

	int time;

	std::shared_ptr<BaseCharacter> player;

	GUN_BEHAVE::BehaveName NextBehave;

protected:
	GUN_BEHAVE::AttackType type;

	void ChackAttack(int level);
	void CheckStep();

public:

};