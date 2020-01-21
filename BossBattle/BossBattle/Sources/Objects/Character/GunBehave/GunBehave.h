#pragma once
#include"../BaseCharacter.h"
#include"../../../System/Timer.h"
#include"../../../System/InputController.h"

namespace GUN_BEHAVE
{
	enum class BehaveName
	{
		NONE = -1,
		WAIT,
		DASH,
		RUN,
		JUMP,
		FALL,
		BREAD1,
		GUN1,
	};

}

struct GunBehave
{
	GunBehave();
	~GunBehave() {};

	virtual void Update(std::shared_ptr<Param> param) = 0;

	int time;

protected:
	GUN_BEHAVE::BehaveName nextBehave;

	void ChackAttack();

public:
	bool IsNextBehave();
	GUN_BEHAVE::BehaveName GetNextBehave();

};