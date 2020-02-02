#pragma once
#include"../BaseScene.h"

//#include"../../Polygon/Sprite/Sprite.h"
//#include"../../Shader/SpriteShader.h"
//#include"../../Polygon/Model/FbxModel.h"
//#include"../../Shader/ModelShader.h"
#include"../../Objects/Character/GunBreaker.h"
#include"../../Objects/Character/SpaceBoss.h"

#include"../../Polygon/ObjectManager.h"

class Battle : public BaseScene
{
	std::shared_ptr<ObjectManager> objectManager;

	ComPtr<ID3D11Buffer> pConstantBuffer;

	std::shared_ptr<GunBreaker> player;
	std::shared_ptr<SpaceBoss> bossEnemy;
	std::shared_ptr<Light> light;

	std::shared_ptr<BaseModel> grid;
	std::shared_ptr<ModelShader> gridShader;

public:
	Battle(ComPtr<ID3D11Device> pDevice);
	~Battle();

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void ObjectUpdate();
	void CollisionUpdate();

	void CollisionCheck();

	//pos1Ç∆pos2ÇÃìñÇΩÇËîªíË
	//lÇ…ãóó£ÇÃ2èÊÇ™ì¸ÇÈ
	bool IsCollision(DirectX::XMFLOAT3 delta, float r, float* l);

	DirectX::XMFLOAT3 NormalizeFloat3(DirectX::XMFLOAT3);

	void SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext);
};