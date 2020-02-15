#include"GunSpecial.h"

GunSpecial::GunSpecial(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
}

void GunSpecial::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time > 2000)
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

	//ChackAttack();

}