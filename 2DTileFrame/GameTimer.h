#pragma once


class GameTimer
{
private:
	float _secondsPerCount;		//윈도우에서 시간 가져올거
	float _deltaTime;	//이전 업데이트부터 현재까지의 시간
	float _prevTime;	//이전 시간 기억

public:
	GameTimer();	//생성자
	~GameTimer();	//소멸자

	void Init();
	void Update();
	float GetDeltaTime();
};