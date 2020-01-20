#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunWait : public GunBehave
{
	GunWait(std::shared_ptr<Param> param);
	~GunWait() {};


	void Update(std::shared_ptr<Param> param);
};
