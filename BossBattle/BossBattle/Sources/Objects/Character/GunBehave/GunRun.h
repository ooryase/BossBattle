#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunRun : public GunBehave
{
	GunRun(std::shared_ptr<Param> param);
	~GunRun() {};


	void Update(std::shared_ptr<Param> param);
};
