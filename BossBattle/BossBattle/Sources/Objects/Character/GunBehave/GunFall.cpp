#include"GunFall.h"

GunFall::GunFall(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param,_player)
{
}

void GunFall::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (param->ground)
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

	CheckStep();
	ChackAttack(0);

}