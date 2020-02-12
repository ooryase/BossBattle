#include"ShiftBread2.h"

ShiftBread2::ShiftBread2(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.y = 0.25f;
	param->speed.x = 0.2f * sin(param->direction.z);
	param->gravity = param->GRAVITY_DEF * 0.8f;

}

void ShiftBread2::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();



	light->Player.x = pos.x + 4.0f * sin(param->direction.z);
	light->Player.y = pos.y - 2.0f;

	float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
	light->PAttenuation.x = 2.0f + temp;
	light->PAttenuation.y = 0.0105f + temp / 100.0f;
	light->PAttenuation.z = 0.0105f + temp / 100.0f;

	param->speed.x *= 0.995f;

	/*if (time > 600)
	{
		if (InputController::getInstance().IsPressKey(DIK_Z) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_A))
		{
			nextBehave = GUN_BEHAVE::BehaveName::BREAD3;
		}
		else if (InputController::getInstance().IsPressKey(DIK_X) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_B))
		{
			nextBehave = GUN_BEHAVE::BehaveName::GUN3;
		}
	}*/

	if (time > 1300)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		if (param->ground)
			nextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			nextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}