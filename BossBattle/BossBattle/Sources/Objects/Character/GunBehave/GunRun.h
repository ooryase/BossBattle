#pragma once
#include"GunBehave.h"


struct GunRun : public GunBehave
{
	GunRun(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunRun() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
