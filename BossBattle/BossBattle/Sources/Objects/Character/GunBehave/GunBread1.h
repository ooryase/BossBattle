#pragma once
#include"GunBehave.h"


struct GunBread1 : public GunBehave
{
	GunBread1(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunBread1() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
