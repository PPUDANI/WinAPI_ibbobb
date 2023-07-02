#include "Lobby.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "BackGround.h"
#include "ContentsEnum.h"
#include "ibb.h"
#include "obb.h"
#include "Medal.h"
#include "LobbyMedal.h"
#include "Warp.h"
#include "Map.h"
#include "LevelDoor.h"
#include "LobbyLevelText.h"
#include "Fade.h"



Lobby::Lobby()
{
	
}

Lobby::~Lobby()
{
}

void Lobby::Start()
{
	// BackGround & Map 생성
	{
		Back = CreateActor<BackGround>(RenderOrder::BackGround);
		Back->Init("Lobby_BackGround.bmp");

		LevelMap = CreateActor<Map>(UpdateOrder::Map);
		LevelMap->Init("Lobby_Map.bmp", ColName);
	}

	// Medal 생성
	{
		for (int i = 0; i < 12; i++)
		{
			LobbyMedal* _Medal = CreateActor<LobbyMedal>(UpdateOrder::Medal);
			Medals.push_back(_Medal);
		}

		Medals[0]->SetPos({ 1050.0f, 546.0f }); // 390
		Medals[1]->SetPos({ 1050.0f, 580.0f });
		Medals[2]->SetPos({ 1050.0f, 614.0f });

		Medals[3]->SetPos({ 1440.0f, 546.0f });
		Medals[4]->SetPos({ 1440.0f, 580.0f });
		Medals[5]->SetPos({ 1440.0f, 614.0f });

		Medals[6]->SetPos({ 1830.0f, 546.0f });
		Medals[7]->SetPos({ 1830.0f, 580.0f });
		Medals[8]->SetPos({ 1830.0f, 614.0f });

		Medals[9]->SetPos({ 2220.0f, 546.0f });
		Medals[10]->SetPos({ 2220.0f, 580.0f });
		Medals[11]->SetPos({ 2220.0f, 614.0f });
	}


	// Level Text 생성
	{
		float _Speed = 5.0f;
		float _MovingHeight = 12.0f;

		float _PosX = 1132.0f;
		float _PosY = 496.0f;
		Level1Text = CreateActor<LobbyLevelText>();
		Level1Text->Init("Level1_Text.bmp");
		Level1Text->LevitationOn();
		Level1Text->Activation();
		Level1Text->SetPos({ _PosX, _PosY });
		Level1Text->SetSpeed(_Speed);
		Level1Text->SetMovingHeight(_MovingHeight);

		_PosX += 390.0f;
		Level2Text = CreateActor<LobbyLevelText>();
		Level2Text->Init("Level2_Text.bmp");
		Level2Text->LevitationOn();
		Level2Text->SetPos({ _PosX, _PosY });
		Level2Text->SetSpeed(_Speed);
		Level2Text->SetMovingHeight(_MovingHeight);

		_PosX += 390.0f;
		Level3Text = CreateActor<LobbyLevelText>();
		Level3Text->Init("Level3_Text.bmp");
		Level3Text->LevitationOn();
		Level3Text->SetPos({ _PosX, _PosY });
		Level3Text->SetSpeed(_Speed);
		Level3Text->SetMovingHeight(_MovingHeight);

		_PosX += 390.0f;
		Level4Text = CreateActor<LobbyLevelText>();
		Level4Text->Init("Level4_Text.bmp");
		Level4Text->LevitationOn();
		Level4Text->SetPos({ _PosX, _PosY });
		Level4Text->SetSpeed(_Speed);
		Level4Text->SetMovingHeight(_MovingHeight);
	}

	// Warp 생성
	{
		LobbyWarp = CreateActor<Warp>(UpdateOrder::Warp);
		LobbyWarp->SetStarNumber(12);
		LobbyWarp->SetWorpDir(WarpDir::Horizontal);
		LobbyWarp->SetWorpType(WarpType::Common);
		LobbyWarp->SetPos({ 423.0f, 650.0f });
		LobbyWarp->Init();
	}

	// LevelDoor 생성
	{
		Level1Door = CreateActor<LevelDoor>();
		Level1Door->SetPos({ 1131.0f, 610.0f });
	}
}

void Lobby::Update(float _DeltaTime)
{
	ZoomScale -= 0.5f * _DeltaTime;
	if (1.0f > ZoomScale)
	{
		ZoomScale = 1.0f;
	}

	if (true == Level1Door->IsPlayerGoIn())
	{
		Level1On = true;
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

void Lobby::LevelStart(GameEngineLevel* _PrevLevel)
{
	if (false == IsCharacterCreated)
	{
		CreateCharacter();
	}

	LevelPlayerInit();

	// Fade
	{
		LobbyStartFade = CreateActor<Fade>();
		LobbyStartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
		LobbyStartFade->SetFadeSpeed(500.0f);
	}

	// 메달 획득 여부 판단
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
	ibbPlayer = ibb::GetMainibb();
	ibbPlayer->SetGroundTexture(ColName);
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });
	ibbPlayer->ChangeState(PlayerState::Fall);
	ibbPlayer->SetDir(PlayerDir::Right);

	// obb
	if (obbPlayer == nullptr)
	{
		MsgBoxAssert("ibbPlayer가 세팅되지 않았습니다.")
	}
	obbPlayer = obb::GetMainobb();
	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });
	obbPlayer->ChangeState(PlayerState::Fall);
	obbPlayer->SetDir(PlayerDir::Right);
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


void Lobby::LobbySettingInit()
{
	Level1On = false;
	Level2On = false;
	Level3On = false;
	EndFadeInit = false;
}