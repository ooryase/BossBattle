#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunJump : public GunBehave
{
	GunJump(std::shared_ptr<Param> param);
	~GunJump() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
