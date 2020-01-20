#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunDash : public GunBehave
{
	GunDash(std::shared_ptr<Param> param);
	~GunDash() {};


	void Update(std::shared_ptr<Param> param);
};
