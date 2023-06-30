#include "Lobby.h"

#include "BackGround.h"
#include "ContentsEnum.h"
#include "Map.h"

Lobby::Lobby()
{
}

Lobby::~Lobby()
{
}

void Lobby::Start()
{

}

void Lobby::Update(float _DeltaTime)
{

}

void Lobby::Render(float _DeltaTime)
{

}

void Lobby::LevelStart(GameEngineLevel* _PrevLevel)
{
	//Back = CreateActor<BackGround>(UpdateOrder::BackGround);
	//Back->Init("LobbyBackGround.bmp");

	LobbyMap = CreateActor<Map>(UpdateOrder::Map);
	LobbyMap->Init("Lobby.Bmp", "Lobby_Collision.Bmp");

}

void Lobby::LevelEnd(GameEngineLevel* _NextLevel)
{

}