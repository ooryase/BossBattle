#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunBread2 : public GunBehave
{
	GunBread2(std::shared_ptr<Param> param);
	~GunBread2() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};