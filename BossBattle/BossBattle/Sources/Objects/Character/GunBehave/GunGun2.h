#pragma once
#include"GunBehave.h"


struct GunGun2 : public GunBehave
{
	GunGun2(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunGun2() {};

	DirectX::XMFLOAT3 shootPos;
	bool shootFlag;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
