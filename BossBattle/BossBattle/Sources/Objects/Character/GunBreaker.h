#pragma once
#include "BaseCharacter.h"
#include "../Effect/BaseEffect.h"
#include "GunBehave/GunBehave.h"

class GunBreaker : public BaseCharacter
{
	std::shared_ptr<Sprite> hpGauge;
	std::shared_ptr<Sprite> hpFrame;
	std::shared_ptr<Sprite> GBFrame;
	std::shared_ptr<Sprite> gunGauge;
	std::shared_ptr<Sprite> breadGauge;
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

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 _offset, DirectX::XMFLOAT3 _scale,
		std::shared_ptr<Sprite> _sprite);


public:
	void CollisionWallUpdate(float wall);
	void CollisionDamage(DParam* dParam) override;
	void AttackHit(int type, int quantity) override;
	void SetEffectReserved(std::shared_ptr<BaseEffect> _obj) override;
};