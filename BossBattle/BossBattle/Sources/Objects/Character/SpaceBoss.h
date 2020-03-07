#pragma once
#include "BaseCharacter.h"
#include"../../Scenes/Camera.h"

class SpaceBoss : public BaseCharacter
{
protected:
	const float scaleDefault;

	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<Sprite> frame;
	std::shared_ptr<SpriteShader> gaugeShader;

	int waitTime;
	int phaseNum;

	enum class DecisionBehavePhase
	{
		WAIT_NEXT,
		MOVE,
		ATTACK,
	} decisionBehave;

	int moveCount;

	enum class BehaveName
	{
		AWAKE,
		BEAM,
		BEAM2,
		SLASH,
		PHASE2_ATTACK,
		PHASE3_ATTACK,
		PHASE3_ATTACK_STEP2,
		WAIT,
		LAND,
		LEAVE,
		DEAD,
	} behave;
	int behaveStep;

	DirectX::XMFLOAT2 posBehave;
	BehaveName phase3Attack;
	DirectX::XMFLOAT2 phase3SetPos;

	enum AnimNumber
	{
		AWAKE,
		BEAM,
		ROAR1,
		ROAR2,
		SLASH,
		WAIT
	} animNum;
	int behaveTime;
	int animSpeedDiv;

	std::shared_ptr<Camera> camera;

	float shaderGSLength;
	FbxTime modelAnimTimeCount;

public:

	SpaceBoss(std::shared_ptr<ObjectManager> objectManager, std::shared_ptr<Light> _light, 
		std::vector< std::shared_ptr< BaseEffect>>& _effectReserves ,
		std::vector< std::shared_ptr< BaseCharacter>>& _enemyReserves,
		std::shared_ptr<Camera> _camera);
	~SpaceBoss();

	//std::shared_ptr<GunBehave> behave;

	void Update();
protected:
	void UpdateAwake();
	void UpdateBeam();
	void UpdateSlash();
	void UpdateLand();
	void UpdateLeave();
	void UpdateWait();
	void UpdatePhase2Attack();
	void UpdatePhase3Attack();
	void UpdatePhase3AttackStep2();
	void UpdateDead();


	virtual void SetColor(DirectX::XMVECTOR* color, DirectX::XMVECTOR* edgeColor);
	virtual void SetNextBehave();
	void DecisionAttackBehave();
	DirectX::XMFLOAT2 GetAvatarPos();

	void SetBehave(BehaveName setBehave);
public:
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) override;
	void DrawGauge(ComPtr<ID3D11DeviceContext> pDeviceContext);

protected:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext) override;

	void DrawSetGauge(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 _offset, DirectX::XMFLOAT3 _scale,
		DirectX::XMVECTOR _weight,
		std::shared_ptr<Sprite> _sprite);
};