#pragma once
#include "BaseCharacter.h"
//#include "GunBehave/GunBehave.h"

class SpaceBoss : public BaseCharacter
{
	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<SpriteShader> gaugeShader;

	enum class BehabeName
	{
		AWAKE,
		BEAM,
		ROAR1,
		ROAR2,
		SLASH,
		WAIT
	} behave;

	enum AnimNumber
	{
		AWAKE,
		BEAM,
		ROAR1,
		ROAR2,
		SLASH,
		WAIT
	} animNum;
	int behaveTime;
	int animSpeedDiv;

public:

	SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light, 
		std::vector< std::shared_ptr< BaseEffect>>& _effectReserves);
	~SpaceBoss();

	//std::shared_ptr<GunBehave> behave;

	void Update();
private:
	void UpdateAwake();
public:
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);
};