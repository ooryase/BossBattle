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

	//QueryPerformanceCounter(&NowTime);//���݂̎��Ԃ��擾
	////Sleep�����鎞��ms = 1�t���[���ڂ��猻�݂̃t���[���܂ł̕`��ɂ�����ׂ�����ms - 1�t���[���ڂ��猻�݂̃t���[���܂Ŏ��ۂɂ�����������ms
	////                  = (1000ms / 60)*�t���[���� - (���݂̎���ms - 1�t���[���ڂ̎���ms)
	//SleepTime = static_cast<DWORD>((1000.0 / 60.0) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);


	//--------------------------���ύX��--------------------------
	// �e�L�X�g�̕`��
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
	if (deltaTime < 30)
		return deltaTime;
	else
		return 30;
}

Timer& Timer::GetInstance()
{
	static Timer instance;
	return instance;
}