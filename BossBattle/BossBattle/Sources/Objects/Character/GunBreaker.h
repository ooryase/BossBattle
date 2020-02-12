#pragma once
#include "BaseCharacter.h"
#include "../Effect/BaseEffect.h"
#include "GunBehave/GunBehave.h"

enum AttackType
{
	BREAD,
	GUN
};

class GunBreaker : public BaseCharacter
{
	//0 : Gun , 1 : Bread
	int typeGauge[2];

	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<SpriteShader> gaugeShader;

public:

	GunBreaker( std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light,
		std::vector< std::shared_ptr< BaseEffect>>& _playerEffectReserves);
	~GunBreaker();

	std::shared_ptr<GunBehave> behave;

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);


public:
	void AttackHit(int type, int quantity) override;
	void SetEffectReserved(std::shared_ptr<BaseEffect> _obj) override;
};