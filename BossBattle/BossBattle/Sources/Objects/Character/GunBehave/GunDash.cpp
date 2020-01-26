#include"GunDash.h"

GunDash::GunDash(std::shared_ptr<Param> param) : GunBehave()
{
}

void GunDash::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x = 0.00f;
	nextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		param->speed.x += -0.04f;
		param->direction.z = -DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::NONE;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		param->speed.x += 0.04f;
		param->direction.z = DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::NONE;
	}

	if (InputController::getInstance().IsPressKey(DIK_SPACE) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_Y))
	{
		nextBehave = GUN_BEHAVE::BehaveName::JUMP;
	}

	ChackAttack();

	if(time > 80)
		nextBehave = GUN_BEHAVE::BehaveName::RUN;
}