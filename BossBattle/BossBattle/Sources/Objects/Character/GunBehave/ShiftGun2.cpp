#include"ShiftGun2.h"
#include"../../Effect/GunBreaker/EffectGun.h"

ShiftGun2::ShiftGun2(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	type = GUN_BEHAVE::AttackType::GUN;

	shootFlag = false;
	param->gravity = param->GRAVITY_DEF * 0.1f;
}

void ShiftGun2::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time < 15 * 16)
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
			param->gravity = param->GRAVITY_DEF * 0.85f;
			shootPos = pos;
			param->speed.y = 0.3f;
			player->SetEffectReserved(std::make_shared<EffectGun>(player->GetObjectManager(), player));
			shootFlag = true;
			param->speed.x = -0.25f * sin(param->direction.z);
		}
		float temp = static_cast<float>(time - 15 * 16) * 1.1f;
		light->SetPointLight(
			DirectX::XMFLOAT4(pos.x + temp * sin(param->direction.z), pos.y - 2.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 0.0f),
			DirectX::XMFLOAT4(1.0f, 0.0105f, 0.0105f, 0.0f)
		);
	}


	param->speed.x *= 0.991f;

	if (time > 1000)
	{
		CheckStep();
	}

	if (time > 1200)
	{
		if (param->ground)
			NextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			NextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}