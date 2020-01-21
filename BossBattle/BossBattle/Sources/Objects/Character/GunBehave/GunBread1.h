#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunBread1 : public GunBehave
{
	GunBread1(std::shared_ptr<Param> param);
	~GunBread1() {};


	void Update(std::shared_ptr<Param> param);
};
