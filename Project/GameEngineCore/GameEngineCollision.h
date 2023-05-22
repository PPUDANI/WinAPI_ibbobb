#pragma once

#include <GameEngineCore/GameEngineObject.h>
class GameEngineActor;
class GameEngineCollision : public GameEngineObject
{
public:
	GameEngineCollision();
	~GameEngineCollision();

	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;
protected:

private:

};

