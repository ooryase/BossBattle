#pragma once
#include"SpaceBoss.h"

class SpaceBossAvatar : public SpaceBoss
{
	BehaveName toBehave;

public:
	SpaceBossAvatar(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
		std::vector< std::shared_ptr< BaseEffect>>& _effectReserves,
		std::vector< std::shared_ptr< BaseCharacter>>& _charaReserves,
		std::shared_ptr<Camera> _camera,
		DirectX::XMFLOAT2 _posBehabe, BehaveName _toBehave, int _startTime);

private:
	void SetColor(DirectX::XMVECTOR* color, DirectX::XMVECTOR* edgeColor) override;
	void SetNextBehave() override;

};