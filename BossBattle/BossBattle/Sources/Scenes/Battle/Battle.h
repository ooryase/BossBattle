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

	vector< shared_ptr< BaseCharacter>> enemies;
	vector< shared_ptr< BaseCharacter>> enemyReserves;


	std::unique_ptr<Space> backGround;

	std::shared_ptr<Sprite> uiPause;
	std::shared_ptr<Sprite> uiClear;
	std::shared_ptr<Sprite> uiGameOver;
	std::shared_ptr<Sprite> uiRetry;
	std::shared_ptr<Sprite> uiTitle;
	std::shared_ptr<SpriteShader> uiShader;
	bool isSelectRetry;
	bool isPause;

	std::shared_ptr<BaseModel> fade;
	std::shared_ptr<ModelShader> fadeShader;



	enum class Phase
	{
		START,
		BATTLE,
		FINISH,
	} phase;
	int phaseTime;

public:
	Battle(ComPtr<ID3D11Device> pDevice);
	~Battle();

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	void ChangePause();
	void UpdateSelect();

	void UpdateObjects();
	void UpdateCollision();
	void UpdatePhase();

	void EndUpdateEffects(vector< shared_ptr< BaseEffect>>* effects, vector< shared_ptr< BaseEffect>>* effectReserves);

	template<typename U, typename V>
	void EndUpdateVector(vector< shared_ptr< U>>* vectorMain, vector< shared_ptr< V>>* vectorReserves);

	void CheckCollision(std::shared_ptr<BaseObject> obj1, std::shared_ptr<BaseObject> obj2);
	void CheckCollisionDamageObjcts(vector< shared_ptr< BaseEffect>> dObjs, std::shared_ptr<BaseCharacter> chara);
	void CheckCollisionDamageObj(std::shared_ptr<BaseEffect> dObj, std::shared_ptr<BaseCharacter> chara);

	//pos1Ç∆pos2ÇÃìñÇΩÇËîªíË
	//lÇ…ãóó£ÇÃ2èÊÇ™ì¸ÇÈ
	bool IsCollide(DirectX::XMFLOAT3 delta, float r, float* l);

	void SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void SetLight(ComPtr<ID3D11DeviceContext> pDeviceContext);

	void DrawFade(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawUI(
		ComPtr<ID3D11DeviceContext> pDeviceContext,
		DirectX::XMMATRIX _world,
		DirectX::XMVECTOR _weight,
		std::shared_ptr<Sprite> _sprite);


	DirectX::XMFLOAT3 NormalizeFloat3(DirectX::XMFLOAT3 f3);

};