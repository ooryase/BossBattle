#pragma once
#include"../BaseScene.h"

//#include"../../Polygon/Sprite/Sprite.h"
//#include"../../Shader/SpriteShader.h"
//#include"../../Polygon/Model/FbxModel.h"
//#include"../../Shader/ModelShader.h"
#include"../../Objects/Character/GunBreaker.h"
#include"../../Polygon/ObjectManager.h"

class Battle : public BaseScene
{
	std::shared_ptr<ObjectManager> objectManager;

	ID3D11Buffer* pConstantBuffer;

	std::shared_ptr<GunBreaker> player;
	std::shared_ptr<Light> light;

	std::shared_ptr<BaseModel> grid;
	std::shared_ptr<ModelShader> gridShader;

public:
	Battle(ID3D11Device* pDevice);
	~Battle();

	void Update();
	void EndUpdate();
	void Draw(ID3D11DeviceContext* pDeviceContext);
};