#pragma once
#include"../BaseCharacter.h"

namespace GUN_BEHAVE
{
	enum class BehaveName
	{
		NONE = -1,
		WAIT,
		DASH,
		JUMP,
	};

}

struct GunBehave
{
	GunBehave();
	~GunBehave() {};

	virtual void Update(std::shared_ptr<Param> param) = 0;

protected:
	GUN_BEHAVE::BehaveName nextBehave;

public:
	bool IsNextBehave();
	GUN_BEHAVE::BehaveName GetNextBehave();

};