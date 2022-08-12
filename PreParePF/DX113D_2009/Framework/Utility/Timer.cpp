#include "Framework.h"

Timer::Timer()
	: frameRate(0), frameCount(0), timeElapsed(0), oneSecCount(0),
	runTime(0), lockFPS(0)
{
	//1초동안 CPU진동수를 반환하는 함수
	QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);

	//현재 CPU진동수
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

	timeScale = 1.0f / (float)periodFrequency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	timeElapsed = (float)(curTime - lastTime) * timeScale;

	if (lockFPS != 0)
	{
		while (timeElapsed < (1.0f / lockFPS))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
			timeElapsed = (float)(curTime - lastTime) * timeScale;
		}
	}

	lastTime = curTime;

	frameCount++;
	oneSecCount += timeElapsed;

	if (oneSecCount >= 1.0f)
	{
		frameRate = frameCount;
		frameCount = 0;
		oneSecCount = 0;
	}

	runTime += timeElapsed;
}

void Timer::Render(HDC hdc)
{
	wstring str;
	str = L"FPS : " + to_wstring(frameRate);
	TextOut(hdc, 0, 0, str.c_str(), (int)str.size());
	str = L"RunTime : " + to_wstring(runTime);
	TextOut(hdc, 0, 20, str.c_str(), (int)str.size());
	str = L"Elapsed : " + to_wstring(timeElapsed);
	TextOut(hdc, 0, 40, str.c_str(), (int)str.size());
}
