#pragma once
#include"../System/Resource.h"

struct CONSTANT_BUFFER
{
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
};

enum class SceneName
{
	NONE,
	TITLE,
	SELECT,
	BATTLE,
	CLEAR
};

class BaseScene
{
protected:
	SceneName nextScene;

public:
	BaseScene();

	virtual void Update() = 0;
	virtual void EndUpdate() = 0;
	virtual void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;

	SceneName GetNextScene() const { return nextScene; }
};