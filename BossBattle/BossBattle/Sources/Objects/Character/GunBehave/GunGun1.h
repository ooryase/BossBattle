#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunGun1 : public GunBehave
{
	GunGun1(std::shared_ptr<Param> param);
	~GunGun1() {};

	DirectX::XMFLOAT3 shootPos;
	bool shootFlag;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
