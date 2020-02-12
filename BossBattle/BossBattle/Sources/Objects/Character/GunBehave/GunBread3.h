#pragma once
#include"GunBehave.h"


struct GunBread3 : public GunBehave
{
	GunBread3(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunBread3() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};