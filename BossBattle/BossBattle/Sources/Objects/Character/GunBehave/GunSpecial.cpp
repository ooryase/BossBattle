#include"GunSpecial.h"

GunSpecial::GunSpecial(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	param->speed = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);
	param->gravity = 0.0f;
}

void GunSpecial::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	if (time > 1600)
		NextBehave = GUN_BEHAVE::BehaveName::WAIT;


}