#pragma once
#include"../System/Resource.h"
#include"Camera.h"


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
	CLEAR,
	QUIT,
};

class BaseScene
{
protected:
	SceneName nextScene;

	ComPtr<ID3D11Buffer> pConstantBuffer;
	ComPtr<ID3D11Buffer> lightConstantBuffer;

	std::shared_ptr<Camera> camera;
	DirectX::XMMATRIX proj;

public:
	BaseScene();

	virtual void Update() = 0;
	virtual void EndUpdate() = 0;
	virtual void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;
	virtual void DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;

	SceneName GetNextScene() const { return nextScene; }

private:
	virtual void SetViewProj(ComPtr<ID3D11DeviceContext> pDeviceContext) = 0;

};