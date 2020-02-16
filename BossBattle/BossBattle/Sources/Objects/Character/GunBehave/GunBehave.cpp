#include"GunBehave.h"

GunBehave::GunBehave(std::shared_ptr<Param> _param, std::shared_ptr< BaseCharacter> _player) :
	param(_param),
	player(_player)
{
	NextBehave = GUN_BEHAVE::BehaveName::NONE;
	type = GUN_BEHAVE::AttackType::NONE;
	time = 0;
}

GunBehave::~GunBehave()
{
	param->gravity = param->GRAVITY_DEF;
}


void GunBehave::ChackAttack(int level)
{
	if ((InputController::getInstance().IsPushKey(DIK_C) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_X)) &&
		//param->typeGauge[GUN_BEHAVE::AttackType::GUN] == 100 &&
		//param->typeGauge[GUN_BEHAVE::AttackType::BREAD] == 100)
		true)
	{
		NextBehave = GUN_BEHAVE::BehaveName::SPECIAL;
	}

	if (InputController::getInstance().IsPushKey(DIK_Z) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_A))
	{
		if(type != GUN_BEHAVE::AttackType::GUN)
			NextBehave = static_cast<GUN_BEHAVE::BehaveName>(GUN_BEHAVE::BehaveName::BREAD1 + level);
		else
			NextBehave = static_cast<GUN_BEHAVE::BehaveName>(GUN_BEHAVE::BehaveName::SHIFT_BREAD1 + level - 1);

	}
	else if (InputController::getInstance().IsPushKey(DIK_X) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_B))
	{
		if (type != GUN_BEHAVE::AttackType::BREAD)
			NextBehave = static_cast<GUN_BEHAVE::BehaveName>(GUN_BEHAVE::BehaveName::GUN1 + level);
		else
			NextBehave = static_cast<GUN_BEHAVE::BehaveName>(GUN_BEHAVE::BehaveName::SHIFT_GUN1 + level - 1);
	}
}

void GunBehave::CheckStep()
{
	if (InputController::getInstance().IsPushKey(DIK_LSHIFT) ||
		InputController::getInstance().IsPushButtom(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			param->direction.z = -DirectX::XM_PIDIV2;
		}
		if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			param->direction.z = DirectX::XM_PIDIV2;
		}

		NextBehave = GUN_BEHAVE::BehaveName::STEP;
	}

}
