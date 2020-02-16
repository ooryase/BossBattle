#include"GunGun3.h"

GunGun3::GunGun3(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	type = GUN_BEHAVE::AttackType::GUN;

	shootFlag = false;
	param->speed.y = 0.0f;
	param->gravity = param->GRAVITY_DEF * 0.4f;

}

void GunGun3::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
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
			param->speed.y = 0.08f;
			shootFlag = true;
		}
		float temp = static_cast<float>(time - 15 * 16) * 1.1f;

		light->Player.x = pos.x + temp * sin(param->direction.z);
		light->Player.y = pos.y - 2.0f;

		light->PAttenuation.x = 1.0f;
		light->PAttenuation.y = 0.0105f;
		light->PAttenuation.z = 0.0105f;
	}


	param->speed.x *= 0.99f;

	if (time > 800)
	{
		CheckStep();
	}

	if (time > 1100)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		NextBehave = GUN_BEHAVE::BehaveName::WAIT;
	}
}