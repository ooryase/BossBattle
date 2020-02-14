#pragma once
#include"../BaseScene.h"

#include"../../Objects/Character/GunBreaker.h"
#include"../../Objects/Character/SpaceBoss.h"
#include"../../Objects/BaseObject.h"
#include"../../Objects/Effect/BaseEffect.h"
#include"../../Objects/BackGround/Space.h"

#include"../../Polygon/ObjectManager.h"

using std::shared_ptr;
using std::vector;

class Battle : public BaseScene
{
	shared_ptr<ObjectManager> objectManager;

	shared_ptr<GunBreaker> player;
	shared_ptr<SpaceBoss> bossEnemy;
	shared_ptr<Light> light;

	vector< shared_ptr< BaseEffect>> playerEffects;
	vector< shared_ptr< BaseEffect>> playerEffectReserves;

	vector< shared_ptr< BaseEffect>> enemyEffects;
	vector< shared_ptr< BaseEffect>> enemyEffectReserves;

	std::unique_ptr<Space> backGround;

	enum class Phase
	{
		START,
		BATTLE,
		FINISH,
	} phase;
	int phaseTime;
	DirectX::XMFLOAT3 eyeLookAt;
	DirectX::XMFLOAT3 eyeDirection;
	float eyeLenght;

public:
	Battle(ComPtr<ID3D11Device> pDevice);
	~Battle();

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void UpdateObjects();
	void UpdateCollision();
	void UpdateCamera();

	void CheckCollision(std::shared_ptr<BaseObject> obj1, std::shared_ptr<BaseObject> obj2);
	void CheckCollisionDamageObj(std::shared_ptr<BaseEffect> dObj, std::shared_ptr<BaseCharacter> chara);

	//pos1��pos2�̓����蔻��
	//l�ɋ�����2�悪����
	bool IsCollide(DirectX::XMFLOAT3 delta, float r, float* l);

	void SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void SetLight(ComPtr<ID3D11DeviceContext> pDeviceContext);

	DirectX::XMFLOAT3 NormalizeFloat3(DirectX::XMFLOAT3 f3);

};