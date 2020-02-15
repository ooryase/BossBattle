#pragma once
#include"GunBehave.h"


struct GunStep : public GunBehave
{
	GunStep(std::shared_ptr<Param> param, std::shared_ptr<BaseCharacter> _player);
	~GunStep() {};


	void Update(DirectX::XMFLOAT3 pos, std::shared_ptr<Light> light);
};
