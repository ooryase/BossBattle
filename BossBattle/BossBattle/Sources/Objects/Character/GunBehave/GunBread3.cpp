#include"GunBread3.h"

GunBread3::GunBread3(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.y = 0.26f;
	param->gravity = param->GRAVITY_DEF * 0.7f;

}

void GunBread3::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	light->Player.x = pos.x + 4.0f * sin(param->direction.z);
	light->Player.y = pos.y - 2.0f;

	float temp = static_cast<float>((time + 200) * (time - 1000)) / 360000.0f;
	light->PAttenuation.x = 2.0f + temp;
	light->PAttenuation.y = 0.0105f + temp / 100.0f;
	light->PAttenuation.z = 0.0105f + temp / 100.0f;

	param->speed.x *= 0.995f;



	if (time > 1400)
	{
		light->Player.x = 1000.0f;
		light->Player.y = 1000.0f;

		if(param->ground)
			nextBehave = GUN_BEHAVE::BehaveName::WAIT;
		else
			nextBehave = GUN_BEHAVE::BehaveName::FALL;

	}
}