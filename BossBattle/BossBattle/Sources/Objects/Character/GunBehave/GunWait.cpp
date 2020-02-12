#include"GunWait.h"

GunWait::GunWait(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
}

void GunWait::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x = 0.00f;
	nextBehave = GUN_BEHAVE::BehaveName::NONE;

	if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		param->speed.x += -0.01f;
		param->direction.z = -DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::DASH;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		param->speed.x += 0.01f;
		param->direction.z = DirectX::XM_PIDIV2;
		nextBehave = GUN_BEHAVE::BehaveName::DASH;
	}

	if (InputController::getInstance().IsPressKey(DIK_SPACE) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_Y))
	{
		nextBehave = GUN_BEHAVE::BehaveName::JUMP;
	}

	ChackAttack();

}