#pragma once
#include"GunBehave.h"


struct GunJump : public GunBehave
{
	GunJump(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunJump() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
