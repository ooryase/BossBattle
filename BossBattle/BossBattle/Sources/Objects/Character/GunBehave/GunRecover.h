#pragma once
#include"GunBehave.h"


struct GunRecover : public GunBehave
{
	GunRecover(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunRecover() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
