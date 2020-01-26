#include"GunFall.h"

GunFall::GunFall(std::shared_ptr<Param> param) : GunBehave()
{
}

void GunFall::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (param->ground)
		nextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		param->speed.x += -0.0003f;
		param->direction.z = -DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		param->speed.x += 0.0003f;
		param->direction.z = DirectX::XM_PIDIV2;
	}

	ChackAttack();

}