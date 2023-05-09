#pragma once
#include "GameEngineObject.h"
#include "GameEngineActor.h"

#include <list>
#include <map>

class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineCore;

public:
	GameEngineLevel();
	~GameEngineLevel();

	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	void CreateActor(int _Order = 0)
	{
		std::list<GameEngineActor*>& GroupList = AllActors[_Order];
		GameEngineActor* NewActor = new ActorType();
		ActorInit(NewActor);
		GroupList.push_back(NewActor);
	}

protected:

private:
	std::map<int, std::list<GameEngineActor*>> AllActors;

	void ActorInit(GameEngineActor* _Actor);
	void ActorUpdate(float _Delta);
	void ActorRender();
};

