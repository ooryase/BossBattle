#include"../BaseEffect.h"
#include"../../Character/BaseCharacter.h"
#pragma once

class BossBeam : public BaseEffect
{
	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> modelShader;

	int time;
	int chageTime;
	int endTime;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 direction;

public:

	BossBeam(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

};