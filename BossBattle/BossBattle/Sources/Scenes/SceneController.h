#pragma once
#include<iostream>
#include"BaseScene.h"

class Scenecontroller
{
	std::unique_ptr<BaseScene> scene;

public:
	void Update();
	void Draw();
};