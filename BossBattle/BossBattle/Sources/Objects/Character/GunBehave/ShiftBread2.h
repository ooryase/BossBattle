#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct ShiftBread2 : public GunBehave
{
	ShiftBread2(std::shared_ptr<Param> param);
	~ShiftBread2() {};

	DirectX::XMFLOAT3 shootPos;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
