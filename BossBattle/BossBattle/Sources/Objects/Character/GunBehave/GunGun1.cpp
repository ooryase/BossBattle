#include"GunGun1.h"
#include"../../Effect/GunBreaker/EffectGun.h"

GunGun1::GunGun1(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	type = GUN_BEHAVE::AttackType::GUN;

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
		float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
		light->SetPointLight(
			DirectX::XMFLOAT4(pos.x + 6.0f * sin(param->direction.z), pos.y - 2.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 0.0f),
			DirectX::XMFLOAT4(2.5f + temp, 0.0105f + temp / 100.0f, 0.0105f + temp / 100.0f, 0.0f)
		);

	}
	else
	{
		if (!shootFlag)
		{
			param->speed.y = 0.04f;
			shootPos = pos;
			player->SetEffectReserved(std::make_shared<EffectGun>(player->GetObjectManager(), player));
			shootFlag = true;
		}

		float temp = static_cast<float>(time - 15 * 16);
		light->SetPointLight(
			DirectX::XMFLOAT4(pos.x + temp * sin(param->direction.z), pos.y - 2.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 0.0f),
			DirectX::XMFLOAT4(1.0f, 0.0105f, 0.0105f, 0.0f)
		);
	}


	param->speed.x *= 0.99f;

	if (time > 500)
	{
		ChackAttack(1);
	}


	if (time > 800)
	{
		if (param->ground)
			NextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			NextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}