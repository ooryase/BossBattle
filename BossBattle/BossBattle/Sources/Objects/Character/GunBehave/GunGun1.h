#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunGun1 : public GunBehave
{
	GunGun1(std::shared_ptr<Param> param);
	~GunGun1() {};


	void Update(std::shared_ptr<Param> param);
};
