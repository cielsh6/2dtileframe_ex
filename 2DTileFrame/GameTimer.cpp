#include <Windows.h>
#include "GameTimer.h"


GameTimer::GameTimer()	//������
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);	//CPU ���ɿ� �ٰ��� ���� ������ (���� �ð� �ƴ�, ��� Ƣ����� ������)
	_secondsPerCount = 1.0f / (double)countsPerSec;
}

GameTimer::~GameTimer()	//�Ҹ���
{
}

void GameTimer::Init()	// �����ڿ� �и���Ű�� ���� ������ �����
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	_prevTime = currentTime;

	_deltaTime = 0.0f;
}

void GameTimer::Update()
{
	__int64 currentTime;	//�ϵ���� ī��Ʈ
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	_deltaTime = (currentTime - _prevTime) * _secondsPerCount;	//���� �ð� ���
	_prevTime = currentTime;	//�ϵ���� ī��Ʈ
}

float GameTimer::GetDeltaTime()
{
	return _deltaTime;
}