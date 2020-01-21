#pragma once
#include<iostream>
#include"BaseScene.h"
#include"../System/Timer.h"
#include"../System/Resource.h"

class SceneController
{
	std::unique_ptr<BaseScene> scene;

public:
	SceneController(ID3D11Device* pDevice);
	~SceneController();

	void Update();
	void Draw(ID3D11DeviceContext* pDeviceContext);
	void EndUpdate(ID3D11Device* pDevice);

private:
	void ChackNextScene(ID3D11Device* pDevice);
};