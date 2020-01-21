#pragma once
#include"Resource.h"

class Timer
{
	Timer(const Timer&) = delete;
	Timer operator = (const Timer&) = delete;
	Timer() {};


	LARGE_INTEGER freqTime;
	LARGE_INTEGER nowTime;
	LARGE_INTEGER lastTime;
	int deltaTime;
public:
	
	void Init();

	void Update();
	void Sleep();

	int GetDeltaTime() const;

	static Timer& GetInstance();
};