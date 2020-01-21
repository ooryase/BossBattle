#include"Timer.h"
#include <thread>
#include <chrono>

void Timer::Init()
{
	QueryPerformanceFrequency(&freqTime);
	QueryPerformanceCounter(&nowTime);
	lastTime = nowTime;
	deltaTime = 0;
}

void Timer::Update()
{
	QueryPerformanceCounter(&nowTime);
	deltaTime = static_cast<int>((nowTime.QuadPart - lastTime.QuadPart) * 1000 / freqTime.QuadPart);
	lastTime = nowTime;

	//QueryPerformanceCounter(&NowTime);//現在の時間を取得
	////Sleepさせる時間ms = 1フレーム目から現在のフレームまでの描画にかかるべき時間ms - 1フレーム目から現在のフレームまで実際にかかった時間ms
	////                  = (1000ms / 60)*フレーム数 - (現在の時間ms - 1フレーム目の時間ms)
	//SleepTime = static_cast<DWORD>((1000.0 / 60.0) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);


	//--------------------------★変更↓--------------------------
	// テキストの描画
	//WCHAR wcText1[256] = { 0 };
	//
	//swprintf(wcText1, 256, L"%d", deltaTime);
	//
	//ID2D1RenderTarget* pRT;
	//pRT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, pTextFormat, D2D1::RectF(0, 0, 800, 20), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void Timer::Sleep()
{
	if (deltaTime < 16)
		std::this_thread::sleep_for(std::chrono::microseconds(16 - deltaTime));
}

int  Timer::GetDeltaTime() const
{
	return deltaTime;
}

Timer& Timer::GetInstance()
{
	static Timer instance;
	return instance;
}