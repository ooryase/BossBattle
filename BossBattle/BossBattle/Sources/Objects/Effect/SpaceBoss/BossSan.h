#include"../BaseEffect.h"
#include"../../Character/BaseCharacter.h"
#pragma once

class BossSan : public BaseEffect
{
	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> modelShader;

	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<SpriteShader> spriteShader;


	int time;
	int fallTime;
	int endTime;
	int startTime;

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 direction;

public:

	BossSan(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<BaseCharacter> _player, int _startTime, float posX);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

};