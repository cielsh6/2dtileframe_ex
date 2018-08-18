#include <Windows.h>
#include "GameTimer.h"


GameTimer::GameTimer()	//생성자
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);	//CPU 성능에 근거한 값을 가져옴 (실제 시간 아님, 몇번 튀겼는지 가져옴)
	_secondsPerCount = 1.0f / (double)countsPerSec;
}

GameTimer::~GameTimer()	//소멸자
{
}

void GameTimer::Init()	// 생성자와 분리시키기 위해 별도로 만든다
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	_prevTime = currentTime;

	_deltaTime = 0.0f;
}

void GameTimer::Update()
{
	__int64 currentTime;	//하드웨어 카운트
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	_deltaTime = (currentTime - _prevTime) * _secondsPerCount;	//실제 시간 계산
	_prevTime = currentTime;	//하드웨어 카운트
}

float GameTimer::GetDeltaTime()
{
	return _deltaTime;
}