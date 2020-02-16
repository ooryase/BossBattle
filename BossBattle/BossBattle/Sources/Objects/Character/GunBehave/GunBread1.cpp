#include"GunBread1.h"
#include"../../Effect/GunBreaker/EffectBread1.h"

GunBread1::GunBread1(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param,_player)
{
	type = GUN_BEHAVE::AttackType::BREAD;
	if(!param->ground)
		param->speed.y = 0.025f;

	param->gravity = param->GRAVITY_DEF * 0.1f;

	player->SetEffectReserved(std::make_shared<EffectBread1>(player->GetObjectManager(), player));
}

void GunBread1::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	light->Player.x = pos.x + 4.0f * sin(param->direction.z);
	light->Player.y = pos.y - 2.0f;

	float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
	light->PAttenuation.x = 2.0f + temp;
	light->PAttenuation.y = 0.0105f + temp / 100.0f;
	light->PAttenuation.z = 0.0105f + temp / 100.0f;

	param->speed.x *= 0.99f;

	if (time > 500)
	{
		ChackAttack(1);
		CheckStep();

	}


	if (time > 800)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		if (param->ground)
			NextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			NextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}