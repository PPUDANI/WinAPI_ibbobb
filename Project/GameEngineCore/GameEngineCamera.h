#pragma once
#include "GameEngineRenderer.h"

#include <list>
#include <map>

class GameEngineCamera
{
public:
	GameEngineCamera();
	~GameEngineCamera();

	GameEngineCamera(const GameEngineCamera& _Other) = delete;
	GameEngineCamera(GameEngineCamera&& _Other) noexcept = delete;
	GameEngineCamera& operator=(const GameEngineCamera& _Other) = delete;
	GameEngineCamera& operator=(GameEngineCamera&& _Other) noexcept = delete;
protected:

private:
	std::map<int, std::list<GameEngineRenderer*>> Renderers;
};

