#include"ShiftBread1.h"
#include"../../Effect/GunBreaker/EffectBread2.h"

ShiftBread1::ShiftBread1(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	type = GUN_BEHAVE::AttackType::BREAD;

	param->speed.y = 0.0f;
	param->speed.x = 0.34f * sin(param->direction.z);
	upperFlag = false;
	param->gravity = param->GRAVITY_DEF * 0.1f;

}

void ShiftBread1::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time > 500 && !upperFlag)
	{
		param->gravity = param->GRAVITY_DEF * 0.85f;
		param->speed.y = 0.2f;
		upperFlag = true;
		player->SetEffectReserved(std::make_shared<EffectBread2>(player->GetObjectManager(), player, 4));
	}

	float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
	light->SetPointLight(
		DirectX::XMFLOAT4(pos.x + 4.0f * sin(param->direction.z), pos.y - 2.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 0.0f),
		DirectX::XMFLOAT4(2.5f + temp, 0.0105f + temp / 100.0f, 0.0105f + temp / 100.0f, 0.0f)
	);


	param->speed.x *= 0.985f;

	if (time > 800)
	{
		ChackAttack(2);
	}

	if (time > 1100)
	{

		if (param->ground)
			NextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			NextBehave = GUN_BEHAVE::BehaveName::FALL;
	}
}