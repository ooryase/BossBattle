#pragma once
#include"../BaseScene.h"

#include"../../Polygon/Sprite/Sprite.h"
#include"../../Shader/SpriteShader.h"
#include"../../Polygon/Model/FbxModel.h"
#include"../../Shader/ModelShader.h"
#include"../../Objects/Character/GunBreaker.h"
#include"../../Polygon/ObjectManager.h"

class Title : public BaseScene
{
	std::shared_ptr<Sprite> sprite;
	std::shared_ptr<SpriteShader> spriteShader;

	ID3D11Buffer* pConstantBuffer;

	float x;

	std::shared_ptr<BaseModel> model;
	std::shared_ptr<ModelShader> mShader;

	std::shared_ptr<GunBreaker> player;

	std::shared_ptr<ObjectManager> objectManager;

public:
	Title(ID3D11Device* pDevice);
	~Title();

	void Update();
	void EndUpdate() {};
	void Draw(ID3D11DeviceContext* pDeviceContext);
};