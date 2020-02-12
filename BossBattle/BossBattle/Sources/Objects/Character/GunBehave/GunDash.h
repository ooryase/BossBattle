#pragma once
#include"GunBehave.h"


struct GunDash : public GunBehave
{
	GunDash(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunDash() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
