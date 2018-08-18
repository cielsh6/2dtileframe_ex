#pragma once


class GameTimer
{
private:
	float _secondsPerCount;		//�����쿡�� �ð� �����ð�
	float _deltaTime;	//���� ������Ʈ���� ��������� �ð�
	float _prevTime;	//���� �ð� ���

public:
	GameTimer();	//������
	~GameTimer();	//�Ҹ���

	void Init();
	void Update();
	float GetDeltaTime();
};