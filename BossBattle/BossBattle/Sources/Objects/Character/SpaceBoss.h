#pragma once
#include "BaseCharacter.h"
#include"../../Scenes/Camera.h"

class SpaceBoss : public BaseCharacter
{
	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<Sprite> frame;
	std::shared_ptr<SpriteShader> gaugeShader;

	enum class BehaveName
	{
		AWAKE,
		BEAM,
		ROAR1,
		ROAR2,
		SLASH,
		WAIT
	} behave;
	int behaveStep;

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

	std::shared_ptr<Camera> camera;

public:

	SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light, 
		std::vector< std::shared_ptr< BaseEffect>>& _effectReserves ,
		std::shared_ptr<Camera> _camera);
	~SpaceBoss();

	//std::shared_ptr<GunBehave> behave;

	void Update();
private:
	void UpdateAwake();
	void UpdateBeam();
	void UpdateSlash();
public:
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);
};