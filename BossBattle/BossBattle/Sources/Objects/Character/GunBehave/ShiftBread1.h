#pragma once
#include"GunBehave.h"


struct ShiftBread1 : public GunBehave
{
	ShiftBread1(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~ShiftBread1() {};

	DirectX::XMFLOAT3 shootPos;
	bool upperFlag;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
