#include"GunStep.h"

GunStep::GunStep(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed.x = 0.25f * sin(param->direction.z);
	param->speed.y = 0.0f;
	param->gravity = 0.0f;

}

void GunStep::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	param->speed.x *= 0.97f;

	if (50 < time && time < 300)
		player->SetTag(ObjectTag::STEALTH);
	else
		player->SetTag(ObjectTag::NORMAL);

	if (time > 350)
		NextBehave =
		(param->ground) ? GUN_BEHAVE::BehaveName::WAIT
		: GUN_BEHAVE::BehaveName::FALL;

	
}