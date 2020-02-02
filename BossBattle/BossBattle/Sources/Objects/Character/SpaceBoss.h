#pragma once
#include "BaseCharacter.h"
//#include "GunBehave/GunBehave.h"

class SpaceBoss : public BaseCharacter
{
	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<SpriteShader> gaugeShader;

public:

	SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light);
	~SpaceBoss();

	//std::shared_ptr<GunBehave> behave;

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);
};