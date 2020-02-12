#pragma once
#include"GunBehave.h"


struct GunWait : public GunBehave
{
	GunWait(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunWait() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
