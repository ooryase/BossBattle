#include"ShiftGun2.h"

ShiftGun2::ShiftGun2(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	shootFlag = false;
	param->gravity = param->GRAVITY_DEF * 0.7f;
}

void ShiftGun2::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time < 15 * 16)
	{
		light->Player.x = pos.x + 6.0f * sin(param->direction.z);
		light->Player.y = pos.y - 2.0f;

		float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
		light->PAttenuation.x = 2.0f + temp;
		light->PAttenuation.y = 0.0105f + temp / 100.0f;
		light->PAttenuation.z = 0.0105f + temp / 100.0f;
	}
	else
	{
		if (!shootFlag)
		{
			shootPos = pos;
			param->speed.y = 0.3f;
			shootFlag = true;
			param->speed.x = -0.22f * sin(param->direction.z);
		}
		float temp = static_cast<float>(time - 15 * 16) * 1.1f;

		light->Player.x = pos.x + temp * sin(param->direction.z);
		light->Player.y = pos.y - 2.0f;

		light->PAttenuation.x = 1.0f;
		light->PAttenuation.y = 0.0105f;
		light->PAttenuation.z = 0.0105f;
	}


	param->speed.x *= 0.991f;

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

	if (time > 1000)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		nextBehave = GUN_BEHAVE::BehaveName::WAIT;
	}
}