#include "GameEngineTime.h"

GameEngineTime GameEngineTime::MainTimer;

GameEngineTime::GameEngineTime()
{
	QueryPerformanceFrequency(&Count);
	QueryPerformanceCounter(&Cur);
}

GameEngineTime::~GameEngineTime()
{

}