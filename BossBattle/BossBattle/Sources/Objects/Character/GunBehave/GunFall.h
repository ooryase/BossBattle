#pragma once
#include"GunBehave.h"


struct GunFall : public GunBehave
{
	GunFall(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunFall() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
