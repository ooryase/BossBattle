#include"GunDash.h"

GunDash::GunDash(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
}

void GunDash::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x = 0.00f;
	NextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT) ||
		InputController::getInstance().IsPressThumbL(THUMB_LEFT::LEFT))
	{
		param->speed.x += -0.05f;
		param->direction.z = -DirectX::XM_PIDIV2;
		NextBehave = GUN_BEHAVE::BehaveName::NONE;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT) ||
		InputController::getInstance().IsPressThumbL(THUMB_LEFT::RIGHT))
	{
		param->speed.x += 0.05f;
		param->direction.z = DirectX::XM_PIDIV2;
		NextBehave = GUN_BEHAVE::BehaveName::NONE;
	}

	if (InputController::getInstance().IsPressKey(DIK_SPACE) ||
		InputController::getInstance().IsPressKey(DIK_UP) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_Y) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_UP) ||
		InputController::getInstance().IsPressThumbL(THUMB_LEFT::UP))
	{
		NextBehave = GUN_BEHAVE::BehaveName::JUMP;
	}

	ChackAttack(0);
	CheckStep();

	if(time > 80)
		NextBehave = GUN_BEHAVE::BehaveName::RUN;
}