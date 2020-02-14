#include"../BaseObject.h"
#pragma once

class Space : public BaseObject
{
	std::shared_ptr<BaseModel> grid;
	std::shared_ptr<ModelShader> gridShader;

	std::shared_ptr<Sprite> space;
	std::shared_ptr<SpriteShader> spaceShader;

	std::shared_ptr<Sprite> star;
	std::shared_ptr<SpriteShader> starShader;

	std::shared_ptr<Sprite> black;
	std::shared_ptr<SpriteShader> blackShader;

	int time;

	const int starPosSize;
	struct StarPos
	{
		float radian;
		int time;
		int speed;
	} starPos[200];


public:

	Space(std::shared_ptr<ObjectManager> objectManager);

	void Update();
	void EndUpdate();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawBillBoard(ComPtr<ID3D11DeviceContext> pDeviceContext, DirectX::XMFLOAT3 eyeDirection);

private:
	void DrawSet(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawSetGrid(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawSetStar(ComPtr<ID3D11DeviceContext> pDeviceContext, StarPos pos, DirectX::XMFLOAT3 eyeDirection);
	void DrawSetBlack(ComPtr<ID3D11DeviceContext> pDeviceContext);

public:
	void OnCollisionEnter(ObjectTag _tag, DirectX::XMFLOAT3 delta) ;


};