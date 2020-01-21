#include"GunDash.h"

GunDash::GunDash(std::shared_ptr<Param> param) : GunBehave()
{
}

void GunDash::Update(std::shared_ptr<Param> param)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x = 0.00f;
	nextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT))
	{
		param->speed.x += -0.01f;
		param->direction.z = -DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::NONE;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT))
	{
		param->speed.x += 0.01f;
		param->direction.z = DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::NONE;
	}

	if (InputController::getInstance().IsPressKey(DIK_SPACE))
	{
		nextBehave = GUN_BEHAVE::BehaveName::JUMP;
	}

	ChackAttack();

	if(time > 80)
		nextBehave = GUN_BEHAVE::BehaveName::RUN;
}