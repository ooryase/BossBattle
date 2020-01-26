#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunGun3 : public GunBehave
{
	GunGun3(std::shared_ptr<Param> param);
	~GunGun3() {};

	DirectX::XMFLOAT3 shootPos;
	bool shootFlag;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light);
};