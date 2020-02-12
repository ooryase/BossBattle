#include"GunBehave.h"

GunBehave::GunBehave(std::shared_ptr<Param> _param, std::shared_ptr< BaseCharacter> _player) :
	param(_param),
	player(_player)
{
	nextBehave = GUN_BEHAVE::BehaveName::NONE;
	time = 0;
}

GunBehave::~GunBehave()
{
	param->gravity = param->GRAVITY_DEF;
}


bool GunBehave::IsNextBehave()
{
	return (nextBehave != GUN_BEHAVE::BehaveName::NONE);
}

GUN_BEHAVE::BehaveName GunBehave::GetNextBehave()
{
	return nextBehave;
}

void GunBehave::ChackAttack()
{
	if (InputController::getInstance().IsPressKey(DIK_Z) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_A))
	{
		nextBehave = GUN_BEHAVE::BehaveName::BREAD1;
	}
	else if (InputController::getInstance().IsPressKey(DIK_X) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_B))
	{
		nextBehave = GUN_BEHAVE::BehaveName::GUN1;
	}
}