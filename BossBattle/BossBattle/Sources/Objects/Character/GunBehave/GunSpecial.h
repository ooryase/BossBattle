#pragma once
#include"GunBehave.h"


struct GunSpecial : public GunBehave
{
	GunSpecial(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunSpecial() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
