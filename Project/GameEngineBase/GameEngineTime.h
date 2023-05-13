#pragma once
#include <windows.h>

class GameEngineTime
{
public:
	static GameEngineTime MainTimer;

	GameEngineTime();
	~GameEngineTime();

	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(GameEngineTime&& _Other) noexcept = delete;
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(GameEngineTime&& _Other) noexcept = delete;

	float GetDeltaTime() // DeltaTime 반환 함수
	{
		return FloatDelta;
	}

	void Reset(); // DeltaTime 초기화
	void Update();  // DeltaTime Update

protected:

private:
	LARGE_INTEGER Count = { 0 };
	LARGE_INTEGER Cur = { 0 };
	LARGE_INTEGER Prev = { 0 };
	__int64 Tick;
	double DoubleDelta;
	float FloatDelta;
};

