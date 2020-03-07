#pragma once
#include"../BaseScene.h"

#include"../../Objects/Character/BaseCharacter.h"
#include"../../Polygon/ObjectManager.h"
#include"../../Objects/BackGround/Space.h"

class Title : public BaseScene
{
	std::shared_ptr<ObjectManager> objectManager;

	std::shared_ptr<Light> light;

	std::unique_ptr<Space> backGround;

	std::shared_ptr<BaseModel> fade;
	std::shared_ptr<ModelShader> fadeShader;

	std::shared_ptr<Sprite> texTitle;
	std::shared_ptr<Sprite> texStart;
	std::shared_ptr<Sprite> texQuit;
	std::shared_ptr<SpriteShader> texShader;

	enum class Phase
	{
		START,
		SELECT,
		FINISH,
	} phase;
	int phaseTime;
	bool isSelectStart;

public:
	Title(ComPtr<ID3D11Device> pDevice);
	~Title();

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext);

private:
	//void UpdateObjects();
	void UpdatePhase();

	void SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void SetLight(ComPtr<ID3D11DeviceContext> pDeviceContext);

	void DrawUI(
		ComPtr<ID3D11DeviceContext> pDeviceContext,
		DirectX::XMMATRIX _world,
		DirectX::XMVECTOR _weight,
		std::shared_ptr<Sprite> _sprite);
};