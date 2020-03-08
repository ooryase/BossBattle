#include"GunRecover.h"

GunRecover::GunRecover(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
}

void GunRecover::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time > 2000)
		NextBehave = GUN_BEHAVE::BehaveName::WAIT;

	if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT) ||
		InputController::getInstance().IsPressThumbL(THUMB_LEFT::LEFT))
	{
		param->speed.x += -0.0003f;
		param->direction.z = -DirectX::XM_PIDIV2;
	}
	if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT) ||
		InputController::getInstance().IsPressThumbL(THUMB_LEFT::RIGHT))
	{
		param->speed.x += 0.0003f;
		param->direction.z = DirectX::XM_PIDIV2;
	}

	//ChackAttack();

}