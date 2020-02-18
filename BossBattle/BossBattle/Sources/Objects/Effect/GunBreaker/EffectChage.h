#include"../BaseEffect.h"
#include"../../Character/BaseCharacter.h"
#pragma once

class EffectChage : public BaseEffect
{
	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<SpriteShader> spriteShader;

	int time;
	int endTime;

	float direction;

	struct ParticlePos
	{
		int time;
		float radian;
		bool active;

		ParticlePos(float _radian) :
			radian(_radian), time(0), active(true) {}
	};

	std::vector<ParticlePos> pPos;

	std::shared_ptr<BaseCharacter> useChara;

public:

	EffectChage(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawSetParticle(ComPtr<ID3D11DeviceContext> pDeviceContext, float _radian, int _time);

};