#include"GunSpecial.h"
#include"../../Effect/GunBreaker/EffectChage.h"
#include"../../Effect/GunBreaker/EffectSpecial.h"

GunSpecial::GunSpecial(std::shared_ptr<Param> _param, std::shared_ptr<BaseCharacter> _player)
	: GunBehave(_param, _player)
{
	player->SetEffectReserved(std::make_shared<EffectChage>(player->GetObjectManager(), player));
	param->speed = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);
	param->gravity = 0.0f;
}

void GunSpecial::Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light)
{
	time += Timer::GetInstance().GetDeltaTime();

	//chage 66f

	//shoot 12 - 200

	//end 220

	if (time > 3500)
		NextBehave = GUN_BEHAVE::BehaveName::WAIT;


}