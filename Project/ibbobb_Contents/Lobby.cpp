#include "Lobby.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include "BackGround.h"
#include "ContentsEnum.h"
#include "Warp.h"
#include "Map.h"
#include "ibb.h"
#include "obb.h"

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
	ZoomScale -= 0.5f * _DeltaTime;
	if (1.0f > ZoomScale)
	{
		ZoomScale = 1.0f;
	}

	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("PlayLevel1");
	}

	SubLevel::Update(_DeltaTime);
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(ZoomScale);
}

void Lobby::Render(float _DeltaTime)
{

}

void Lobby::LevelStart(GameEngineLevel* _PrevLevel)
{
	Back = CreateActor<BackGround>(UpdateOrder::BackGround);
	Back->Init("Lobby_BackGround.bmp");
	std::string ColName = "Lobby_Collision.bmp";
	LevelMap = CreateActor<Map>(UpdateOrder::Map);
	LevelMap->Init("Lobby_Map.bmp", ColName);
	

	float DefaultPosX = 100.0f;
	float DefaultPosY = 0.0f;

	// ibb
	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetRatio(2.0f);
	ibbPlayer->SetGroundTexture(ColName);
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });
	
	// obb
	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetRatio(2.0f);
	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });

	ibbPlayer->SetOtherPlayer(dynamic_cast<Player*>(obbPlayer));
	obbPlayer->SetOtherPlayer(dynamic_cast<Player*>(ibbPlayer));

	LobbyWarp = CreateActor<Warp>(UpdateOrder::Warp);
	LobbyWarp->SetStarNumber(12);
	LobbyWarp->SetWorpDir(WarpDir::Horizontal);
	LobbyWarp->SetWorpType(WarpType::Common);
	LobbyWarp->SetPos({ 423.0f, 650.0f });
	LobbyWarp->Init();



	LevelMaxScaleX = 2782.0f;
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(2.0f);
}

void Lobby::LevelEnd(GameEngineLevel* _NextLevel)
{
	ibbPlayer->OverOn();
	obbPlayer->OverOn();
}