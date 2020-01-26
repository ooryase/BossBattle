#include"GunGun1.h"

GunGun1::GunGun1(std::shared_ptr<Param> param) : GunBehave()
{
	shootFlag = false;
}

void GunGun1::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time < 15 * 16)
	{
		light->playerLight.x = pos.x + 6.0f * sin(param->direction.z);
		light->playerLight.y = pos.y - 2.0f;

		float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
		light->playerAttenuation.x = 2.0f + temp;
		light->playerAttenuation.y = 0.0105f + temp / 100.0f;
		light->playerAttenuation.z = 0.0105f + temp / 100.0f;
	}
	else
	{
		if (!shootFlag)
		{
			shootPos = pos;
		}
		float temp = static_cast<float>(time - 15* 16) * 1.1f;

		light->playerLight.x = pos.x + temp * sin(param->direction.z);
		light->playerLight.y = pos.y - 2.0f;

		light->playerAttenuation.x = 1.0f;
		light->playerAttenuation.y = 0.0105f;
		light->playerAttenuation.z = 0.0105f;
	}


	param->speed.x *= 0.99f;

	if (time > 600)
	{
		if (InputController::getInstance().IsPressKey(DIK_Z) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_A))
		{
			nextBehave = GUN_BEHAVE::BehaveName::BREAD2;
		}
		else if (InputController::getInstance().IsPressKey(DIK_X) ||
			InputController::getInstance().IsPressButtom(XINPUT_GAMEPAD_B))
		{
			nextBehave = GUN_BEHAVE::BehaveName::GUN2;
		}
	}

	//param->speed.x = 0.00f;
	//nextBehave = GUN_BEHAVE::BehaveName::WAIT;
	//
	//if (InputController::getInstance().IsPressKey(DIK_LEFT))
	//{
	//	param->speed.x += -0.01f;
	//	param->direction.z = -DirectX::XM_PIDIV2;
	//	nextBehave = GUN_BEHAVE::BehaveName::NONE;
	//}
	//if (InputController::getInstance().IsPressKey(DIK_RIGHT))
	//{
	//	param->speed.x += 0.01f;
	//	param->direction.z = DirectX::XM_PIDIV2;
	//	nextBehave = GUN_BEHAVE::BehaveName::NONE;
	//}
	//
	//if (InputController::getInstance().IsPressKey(DIK_SPACE))
	//{
	//	nextBehave = GUN_BEHAVE::BehaveName::JUMP;
	//}

	if (time > 1000)
	{
		light->playerLight.x = 1000.0f;
		light->playerLight.y = 1000.0f;

		nextBehave = GUN_BEHAVE::BehaveName::WAIT;
	}
}