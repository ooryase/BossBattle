#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunWait : public GunBehave
{
	GunWait(std::shared_ptr<Param> param);
	~GunWait() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Param> param, std::shared_ptr<Light> light);
};
