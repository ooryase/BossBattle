#include"GunBread2.h"

GunBread2::GunBread2(std::shared_ptr<Param> _param) : GunBehave(_param)
{
	if (!param->ground && param->speed.y < 0.0f)
		param->speed.y = 0.015f;

	param->gravity = param->GRAVITY_DEF * 0.2f;
}

void GunBread2::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	light->playerLight.x = pos.x + 4.0f * sin(param->direction.z);
	light->playerLight.y = pos.y - 2.0f;

	float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
	light->playerAttenuation.x = 2.0f + temp;
	light->playerAttenuation.y = 0.0105f + temp / 100.0f;
	light->playerAttenuation.z = 0.0105f + temp / 100.0f;

	param->speed.x *= 0.995f;

	if (time > 600)
	{
		if (InputController::getInstance().IsPressKey(DIK_Z) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_A))
		{
			nextBehave = GUN_BEHAVE::BehaveName::BREAD3;
		}
		else if (InputController::getInstance().IsPressKey(DIK_X) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_B))
		{
			nextBehave = GUN_BEHAVE::BehaveName::SHIFT_GUN2;
		}
	}

	if (time > 1300)
	{
		light->playerLight.x = 1000.0f;
		light->playerLight.y = 1000.0f;

		if (param->ground)
			nextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			nextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}