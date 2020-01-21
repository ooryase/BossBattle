#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunFall : public GunBehave
{
	GunFall(std::shared_ptr<Param> param);
	~GunFall() {};


	void Update(std::shared_ptr<Param> param);
};
