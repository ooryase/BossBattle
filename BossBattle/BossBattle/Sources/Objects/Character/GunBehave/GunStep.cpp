#include"GunStep.h"

GunStep::GunStep(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.x = 0.25f * sin(param->direction.z);
	param->speed.y = 0.0f;
	param->gravity = 0.0f;
}

void GunStep::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x *= 0.97f;

	if (time > 350)
		NextBehave =
		(param->ground) ? GUN_BEHAVE::BehaveName::WAIT
		: GUN_BEHAVE::BehaveName::FALL;

	
	//if (InputController::getInstance().IsPressKey(DIK_LEFT) ||
	//	InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_LEFT))
	//{
	//	param->speed.x += -0.0003f;
	//	param->direction.z = -DirectX::XM_PIDIV2;
	//}
	//if (InputController::getInstance().IsPressKey(DIK_RIGHT) ||
	//	InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_DPAD_RIGHT))
	//{
	//	param->speed.x += 0.0003f;
	//	param->direction.z = DirectX::XM_PIDIV2;
	//}

	//ChackAttack();

}