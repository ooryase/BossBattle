#pragma once
#include<iostream>
#include"BaseScene.h"
#include"../System/Timer.h"
#include"../System/Resource.h"

class SceneController
{
	std::unique_ptr<BaseScene> scene;
	bool isDelete;

public:
	SceneController(ComPtr<ID3D11Device> pDevice);
	~SceneController();

	bool IsDelete() const;
		
	void Update();
	void Draw(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void DrawAfterRadialBlur(ComPtr<ID3D11DeviceContext> pDeviceContext);
	void EndUpdate(ComPtr<ID3D11Device> pDevice);

private:
	void ChackNextScene(ComPtr<ID3D11Device> pDevice);
};