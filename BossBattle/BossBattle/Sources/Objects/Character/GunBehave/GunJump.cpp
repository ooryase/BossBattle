#include"GunJump.h"

GunJump::GunJump(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.y = 0.3f;
	param->ground = false;
}

void GunJump::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	//param->speed.y -= 0.0002f;

	if(param->ground)
		NextBehave = GUN_BEHAVE::BehaveName::WAIT;

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

	if (param->speed.y < 0.0f)
	{
		NextBehave = GUN_BEHAVE::BehaveName::FALL;
	}

	if (InputController::getInstance().IsPressKey(DIK_LSHIFT) ||
		InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		NextBehave = GUN_BEHAVE::BehaveName::STEP;
	}

	ChackAttack(0);

}