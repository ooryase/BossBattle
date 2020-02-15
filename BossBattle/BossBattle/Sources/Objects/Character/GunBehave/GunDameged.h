#pragma once
#include"GunBehave.h"


struct GunDameged : public GunBehave
{
	GunDameged(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunDameged() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
