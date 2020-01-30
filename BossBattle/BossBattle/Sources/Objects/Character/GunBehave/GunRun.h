#pragma once
#include"../GunBreaker.h"
#include"GunBehave.h"


struct GunRun : public GunBehave
{
	GunRun(std::shared_ptr<Param> param);
	~GunRun() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
