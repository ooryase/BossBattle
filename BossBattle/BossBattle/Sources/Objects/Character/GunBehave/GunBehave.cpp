#include"GunBehave.h"

GunBehave::GunBehave()
{
	nextBehave = GUN_BEHAVE::BehaveName::NONE;
	time = 0;
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