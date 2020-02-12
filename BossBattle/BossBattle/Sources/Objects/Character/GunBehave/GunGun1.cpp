#include"GunGun1.h"

GunGun1::GunGun1(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.y = 0.0f;
	shootFlag = false;
	param->gravity = param->GRAVITY_DEF * 0.4f;


}

void GunGun1::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	const int CHARGE_TIME = 15;
	const int TARGET_FRAME_MS = 16;
	if (time < CHARGE_TIME * TARGET_FRAME_MS)
	{
		light->Player.x = pos.x + 6.0f * sin(param->direction.z);
		light->Player.y = pos.y - 2.0f;

		float timeFactor = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
		light->PAttenuation.x = 2.0f + timeFactor;
		light->PAttenuation.y = 0.0105f + timeFactor / 100.0f;
		light->PAttenuation.z = 0.0105f + timeFactor / 100.0f;
	}
	else
	{
		if (!shootFlag)
		{
			param->speed.y = 0.04f;
			shootPos = pos;
			shootFlag = true;
		}
		float temp = static_cast<float>(time - 15* 16) * 1.1f;

		light->Player.x = pos.x + temp * sin(param->direction.z);
		light->Player.y = pos.y - 2.0f;

		light->PAttenuation.x = 1.0f;
		light->PAttenuation.y = 0.0105f;
		light->PAttenuation.z = 0.0105f;
	}


	param->speed.x *= 0.99f;

	if (time > 600)
	{
		if (InputController::getInstance().IsPressKey(DIK_Z) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_A))
		{
			nextBehave = GUN_BEHAVE::BehaveName::SHIFT_BREAD1;
		}
		else if (InputController::getInstance().IsPressKey(DIK_X) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_B))
		{
			nextBehave = GUN_BEHAVE::BehaveName::GUN2;
		}
	}


	if (time > 1000)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		nextBehave = GUN_BEHAVE::BehaveName::WAIT;
	}
}