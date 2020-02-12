#include"../BaseEffect.h"
#include"../../Character/BaseCharacter.h"
#pragma once

class EffectBread1 : public BaseEffect
{
	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<SpriteShader> spriteShader;

	int time;
	int endTime;

	DirectX::XMFLOAT3 direction;

	std::shared_ptr<BaseCharacter> useChara;


public:

	EffectBread1(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

	bool IsNewHit(BaseObject* obj) override;

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

};