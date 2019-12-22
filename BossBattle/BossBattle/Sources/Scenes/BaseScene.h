#pragma once

class BaseScene
{
	enum class SceneName
	{
		TITLE,
		SELECT,
		BATTLE,
		CLEAR
	};


public:
	BaseScene() {};
	~BaseScene() {};

	virtual void Update() = 0;
	virtual void Draw() = 0;
};