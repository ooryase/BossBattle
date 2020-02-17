#pragma once
#include"GunBehave.h"


struct ShiftBread2 : public GunBehave
{
private:
	bool slashFlag;

public:

	ShiftBread2(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~ShiftBread2() {};

	DirectX::XMFLOAT3 shootPos;

	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
