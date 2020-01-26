#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunFall : public GunBehave
{
	GunFall(std::shared_ptr<Param> param);
	~GunFall() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light);
};
