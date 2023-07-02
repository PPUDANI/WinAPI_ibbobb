#include "Lobby.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include "BackGround.h"
#include "ContentsEnum.h"

#include "Medal.h"
#include "LobbyMedal.h"
#include "Fade.h"
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

	Medals[0]->SetPos({ 1050.0f, 546.0f});
	Medals[1]->SetPos({ 1050.0f, 580.0f });
	Medals[2]->SetPos({ 1050.0f, 614.0f });

	Medals[3]->SetPos({ 1440.0f, 546.0f });
	Medals[4]->SetPos({ 1440.0f, 580.0f });
	Medals[5]->SetPos({ 1440.0f, 614.0f });

	Medals[6]->SetPos({ 1830.0f, 546.0f }); // 390
	Medals[7]->SetPos({ 1830.0f, 580.0f });
	Medals[8]->SetPos({ 1830.0f, 614.0f });

	Medals[9]->SetPos({ 2220.0f, 546.0f });
	Medals[10]->SetPos({ 2220.0f, 580.0f });
	Medals[11]->SetPos({ 2220.0f, 614.0f });
}

void Lobby::Update(float _DeltaTime)
{
	ZoomScale -= 0.5f * _DeltaTime;
	if (1.0f > ZoomScale)
	{
		ZoomScale = 1.0f;
	}

	if (true == GameEngineInput::IsDown('1'))
	{
		Level1On = true;
	}
	if (true == GameEngineInput::IsDown('2'))
	{
		Level2On = true;
	}
	if (true == GameEngineInput::IsDown('3'))
	{
		Level3On = true;
	}

	if (true == Level1On)
	{
		Level1Start(_DeltaTime);
	}
	if (true == Level1On)
	{
		Level2Start(_DeltaTime);
	}
	if (true == Level1On)
	{
		Level3Start(_DeltaTime);
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

void Lobby::Level1Start(float _DeltaTime)
{
	if (false == EndFadeInit)
	{
		LobbyEndFade = CreateActor<Fade>();
		LobbyEndFade->Init("FadeBlack.bmp", FadeState::FadeOut);
		LobbyEndFade->SetFadeSpeed(400.0f);

		EndFadeInit = true;
	}

	if (LobbyEndFade == nullptr)
	{
		MsgBoxAssert("LobbyEndFade가 nullptr입니다.")
	}

	if (true == LobbyEndFade->FadeIsEnd())
	{
		GameEngineCore::ChangeLevel("PlayLevel1");
	}
}

void Lobby::Level2Start(float _DeltaTime)
{
	return;
}

void Lobby::Level3Start(float _DeltaTime)
{
	return;
}

void Lobby::LevelStart(GameEngineLevel* _PrevLevel)
{
	Back = CreateActor<BackGround>(RenderOrder::BackGround);
	Back->Init("Lobby_BackGround.bmp");

	LevelMap = CreateActor<Map>(UpdateOrder::Map);
	LevelMap->Init("Lobby_Map.bmp", ColName);

	if (false == IsCharacterCreated)
	{
		CreateCharacter();
	}
	LevelPlayerInit();
	// Warp
	{
		LobbyWarp = CreateActor<Warp>(UpdateOrder::Warp);
		LobbyWarp->SetStarNumber(12);
		LobbyWarp->SetWorpDir(WarpDir::Horizontal);
		LobbyWarp->SetWorpType(WarpType::Common);
		LobbyWarp->SetPos({ 423.0f, 650.0f });
		LobbyWarp->Init();
	}

	// 메달 획득 여부 판단
	for (int i = 0; i < Medal::MedalsByLevelIsAcquired.size(); i++)
	{
		if (true == Medal::MedalsByLevelIsAcquired[i]->IsAcquired())
		{
			Medals[i]->SetAnimation("Acquired");
		}
	}

	LobbyStartFade = CreateActor<Fade>();
	LobbyStartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
	LobbyStartFade->SetFadeSpeed(500.0f);

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

	LobbyStartFade->Death();
	LobbyStartFade = nullptr;

	LobbyEndFade->Death();
	LobbyEndFade = nullptr;

	LobbySettingInit();
}

void Lobby::LevelPlayerInit()
{
	float DefaultPosX = 100.0f;
	float DefaultPosY = 0.0f;

	// ibb
	if (ibbPlayer == nullptr)
	{
		MsgBoxAssert("ibbPlayer가 세팅되지 않았습니다.")
	}

	ibbPlayer->SetGroundTexture(ColName);
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });
	ibbPlayer->ChangeState(PlayerState::Fall);
	ibbPlayer->SetDir(PlayerDir::Left);

	// obb
	if (obbPlayer == nullptr)
	{
		MsgBoxAssert("ibbPlayer가 세팅되지 않았습니다.")
	}
	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });
	obbPlayer->ChangeState(PlayerState::Fall);
	ibbPlayer->SetDir(PlayerDir::Left);
}

void Lobby::LobbySettingInit()
{
	Level1On = false;
	Level2On = false;
	Level3On = false;
	EndFadeInit = false;
}