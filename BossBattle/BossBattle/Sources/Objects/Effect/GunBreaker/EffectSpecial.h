#include"../BaseEffect.h"
#include"../../Character/BaseCharacter.h"
#pragma once

class EffectSpecial : public BaseEffect
{
	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> modelShader;

	int time;
	int endTime;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 direction;

	std::shared_ptr<BaseCharacter> useChara;


public:

	EffectSpecial(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

	bool IsNewHit(BaseObject* obj) override;

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

};