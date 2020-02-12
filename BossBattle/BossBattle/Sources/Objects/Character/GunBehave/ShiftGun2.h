#pragma once
#include"GunBehave.h"


struct ShiftGun2 : public GunBehave
{
	ShiftGun2(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~ShiftGun2() {};

	DirectX::XMFLOAT3 shootPos;
	bool shootFlag;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
