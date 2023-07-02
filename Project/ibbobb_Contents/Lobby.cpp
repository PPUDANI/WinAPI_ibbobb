#include "Lobby.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include "BackGround.h"
#include "ContentsEnum.h"

#include "Medal.h"
#include "LobbyMedal.h"
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
	for (int i = 0; i < 12; i++)
	{
		LobbyMedal* _Medal = CreateActor<LobbyMedal>(UpdateOrder::Medal);
		Medals.push_back(_Medal);
	}

	Medals[0]->SetPos({ 1052.0f, 546.0f});
	Medals[1]->SetPos({ 1052.0f, 578.0f });
	Medals[2]->SetPos({ 1052.0f, 610.0f });

	Medals[3]->SetPos({ 1442.0f, 546.0f });
	Medals[4]->SetPos({ 1442.0f, 578.0f });
	Medals[5]->SetPos({ 1442.0f, 610.0f });

	Medals[6]->SetPos({ 1832.0f, 546.0f }); // 390
	Medals[7]->SetPos({ 1832.0f, 578.0f });
	Medals[8]->SetPos({ 1832.0f, 610.0f });

	Medals[9]->SetPos({ 2222.0f, 546.0f });
	Medals[10]->SetPos({ 2222.0f, 578.0f });
	Medals[11]->SetPos({ 2222.0f, 610.0f });
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

void Lobby::CreateCharacter()
{
	// ibb
	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetRatio(2.0f);

	// obb
	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetRatio(2.0f);

	ibbPlayer->SetOtherPlayer(dynamic_cast<Player*>(obbPlayer));
	obbPlayer->SetOtherPlayer(dynamic_cast<Player*>(ibbPlayer));

	IsCharacterCreated = true;
}


void Lobby::LevelStart(GameEngineLevel* _PrevLevel)
{
	Back = CreateActor<BackGround>(UpdateOrder::BackGround);
	Back->Init("Lobby_BackGround.bmp");

	std::string ColName = "Lobby_Collision.bmp";
	LevelMap = CreateActor<Map>(UpdateOrder::Map);
	LevelMap->Init("Lobby_Map.bmp", ColName);

	if (false == IsCharacterCreated)
	{
		CreateCharacter();
	}

	float DefaultPosX = 100.0f;
	float DefaultPosY = 0.0f;

	ibbPlayer->SetGroundTexture(ColName);
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });

	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });

	// Warp
	{
		LobbyWarp = CreateActor<Warp>(UpdateOrder::Warp);
		LobbyWarp->SetStarNumber(12);
		LobbyWarp->SetWorpDir(WarpDir::Horizontal);
		LobbyWarp->SetWorpType(WarpType::Common);
		LobbyWarp->SetPos({ 423.0f, 650.0f });
		LobbyWarp->Init();
	}

	// ¸Þ´Þ È¹µæ ¿©ºÎ ÆÇ´Ü
	for (int i = 0; i < Medal::MedalsByLevelIsAcquired.size(); i++)
	{
		if (true == Medal::MedalsByLevelIsAcquired[i]->IsAcquired())
		{
			Medals[i]->SetAnimation("Acquired");
		}
	}

	LevelMaxScaleX = 2782.0f;
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(2.0f);
}

void Lobby::LevelEnd(GameEngineLevel* _NextLevel)
{
	Back->Death();
	Back = nullptr;

	LevelMap->Death();
	LevelMap = nullptr;

	LobbyWarp->Death();
	LobbyWarp = nullptr;

	ibbPlayer->OverOn();
	obbPlayer->OverOn();
}