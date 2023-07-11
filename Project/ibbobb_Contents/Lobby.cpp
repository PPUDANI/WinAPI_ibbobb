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
#include "DoorStar.h"
#include "Fade.h"
#include "SoundLoadManager.h"

bool Lobby::Level1ClearValue = false;
bool Lobby::Level2ClearValue = false;
bool Lobby::Level3ClearValue = false;
bool Lobby::Level4ClearValue = false;

bool Lobby::EnterLevel1 = false;
bool Lobby::EnterLevel2 = false;
bool Lobby::EnterLevel3 = false;
bool Lobby::EnterLevel4 = false;

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


	// Level Text Level Rock 생성
	{
		float _Speed = 5.0f;
		float _MovingHeight = 12.0f;

		float _PosX = 1132.0f;
		float _PosY = 496.0f;

		float _LockPosY = 85.0f;

		float _StarPosX = -70.0f;
		float _StarPosY = 20.0f;

		Level1Text = CreateActor<LobbyLevelText>();
		Level1Text->Init("Level1_Text.bmp");
		Level1Text->LevitationOn();
		Level1Text->Activation();
		Level1Text->SetPos({ _PosX, _PosY });
		Level1Text->SetSpeed(_Speed);
		Level1Text->SetMovingHeight(_MovingHeight);

		Level1Star = CreateActor<DoorStar>();
		Level1Star->Init();
		Level1Star->SetPos({ _PosX + _StarPosX, _PosY + _StarPosY });

		_PosX += 388.0f;
		Level2Text = CreateActor<LobbyLevelText>();
		Level2Text->Init("Level2_Text.bmp");
		Level2Text->LevitationOn();
		Level2Text->SetPos({ _PosX, _PosY });
		Level2Text->SetSpeed(_Speed);
		Level2Text->SetMovingHeight(_MovingHeight);

		Level2Star = CreateActor<DoorStar>();
		Level2Star->Init();
		Level2Star->SetPos({ _PosX + _StarPosX, _PosY + _StarPosY });

		Level2Lock = CreateActor<DefaultImage>();
		Level2Lock->Init("Lock.bmp");
		Level2Lock->LevitationOn();
		Level2Lock->SetPos({ _PosX, _PosY + _LockPosY });
		Level2Lock->SetSpeed(_Speed);
		Level2Lock->SetMovingHeight(_MovingHeight);

		_PosX += 390.0f;
		Level3Text = CreateActor<LobbyLevelText>();
		Level3Text->Init("Level3_Text.bmp");
		Level3Text->LevitationOn();
		Level3Text->SetPos({ _PosX, _PosY });
		Level3Text->SetSpeed(_Speed);
		Level3Text->SetMovingHeight(_MovingHeight);

		Level3Star = CreateActor<DoorStar>();
		Level3Star->Init();
		Level3Star->SetPos({ _PosX + _StarPosX, _PosY + _StarPosY });

		Level3Lock = CreateActor<DefaultImage>();
		Level3Lock->Init("Lock.bmp");
		Level3Lock->LevitationOn();
		Level3Lock->SetPos({ _PosX, _PosY + _LockPosY });
		Level3Lock->SetSpeed(_Speed);
		Level3Lock->SetMovingHeight(_MovingHeight);

		_PosX += 390.0f;
		Level4Text = CreateActor<LobbyLevelText>();
		Level4Text->Init("Level4_Text.bmp");
		Level4Text->LevitationOn();
		Level4Text->SetPos({ _PosX, _PosY });
		Level4Text->SetSpeed(_Speed);
		Level4Text->SetMovingHeight(_MovingHeight);

		Level4Star = CreateActor<DoorStar>();
		Level4Star->Init();
		Level4Star->SetPos({ _PosX + _StarPosX, _PosY + _StarPosY });

		Level4Lock = CreateActor<DefaultImage>();
		Level4Lock->Init("Lock.bmp");
		Level4Lock->LevitationOn();
		Level4Lock->SetPos({ _PosX, _PosY + _LockPosY });
		Level4Lock->SetSpeed(_Speed);
		Level4Lock->SetMovingHeight(_MovingHeight);
	}

	// Warp 생성
	{
		LobbyWarp = CreateActor<Warp>(UpdateOrder::Warp);
		LobbyWarp->SetStarNumber(12);
		LobbyWarp->SetWorpDir(WarpDir::Horizontal);
		LobbyWarp->SetPos({ 423.0f, 650.0f });
		LobbyWarp->Init(WarpType::Common);
	}

	// LevelDoor 생성
	{
		Level1Door = CreateActor<LevelDoor>();
		Level1Door->SetPos({ 1131.0f, 610.0f });

		Level2Door = CreateActor<LevelDoor>();
		Level2Door->SetPos({ 1521.0f, 610.0f });
		Level2Door->DeActivation();

		Level3Door = CreateActor<LevelDoor>();
		Level3Door->SetPos({ 1911.0f, 610.0f });
		Level3Door->DeActivation();

		Level4Door = CreateActor<LevelDoor>();
		Level4Door->SetPos({ 2301.0f, 610.0f });
		Level4Door->DeActivation();
	}

	// Sound
	{
		SoundLoadManager::LoadSound("BGM", "LobbyBGM.mp3");
		SoundLoadManager::LoadSound("LevelEffect", "EnterLevel.mp3");
	}
}

void Lobby::Update(float _DeltaTime)
{
	ZoomScale -= 0.5f * _DeltaTime;
	if (1.0f > ZoomScale)
	{
		ZoomScale = 1.0f;
	}

	if (true == Level1Door->IsPlayerEnter())
	{
		EnterLevel1 = true;
	}

	if (true == Level2Door->IsPlayerEnter())
	{
		EnterLevel2 = true;
	}

	if (true == DevelopmentMode)
	{
		if (true == GameEngineInput::IsDown('1') ||
			true == Level1Door->IsPlayerEnter())
		{
			EnterLevel1 = true;
		}
		if (true == GameEngineInput::IsDown('2') ||
			true == Level2Door->IsPlayerEnter())
		{
			EnterLevel2 = true;
		}
		//if (true == GameEngineInput::IsDown('3') ||
		//	true == Level3Door->IsPlayerEnter())
		//{
		//	EnterLevel3 = true;
		//}
		//if (true == GameEngineInput::IsDown('4') ||
		//	true == Level4Door->IsPlayerEnter())
		//{
		//	EnterLevel4 = true;
		//}
	}
	

	if (true == EnterLevel1)
	{
		Level1Start(_DeltaTime);
		return;
	}
	if (true == EnterLevel2)
	{
		Level2Start(_DeltaTime);
		return;
	}
	if (true == EnterLevel3)
	{
		Level3Start(_DeltaTime);
		return;
	}
	if (true == EnterLevel4)
	{
		Level4Start(_DeltaTime);
		return;
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
	// Play BGM
	BGMPlayer = GameEngineSound::SoundPlay("LobbyBGM.mp3");
	BGMPlayer.SetLoop(10);
	BGMPlayer.SetVolume(0.3f);

	if (false == IsCharacterCreated)
	{
		CreateCharacter();
	}

	LevelPlayerInit();

	// Fade
	LobbyStartFade = CreateActor<Fade>();
	LobbyStartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
	LobbyStartFade->SetFadeSpeed(500.0f);

	// 메달 획득 여부 판단
	for (int i = 0; i < Medal::MedalsByLevelIsAcquired.size(); i++)
	{
		if (true == Medal::MedalsByLevelIsAcquired[i]->IsAcquired())
		{
			Medals[i]->SetAnimation("Acquired");
		}
	}

	// 레벨 별 플레이어 좌표 초기화
	if (true == EnterLevel1)
	{
		ibbPlayer->SetPos({ 1107.0f ,540.0f });
		obbPlayer->SetPos({ 1157.0f ,540.0f });
	}
	else if (true == EnterLevel2)
	{
		ibbPlayer->SetPos({ 1497.0f ,540.0f });
		obbPlayer->SetPos({ 1547.0f ,540.0f });
	}
	else if (true == EnterLevel3)
	{
		ibbPlayer->SetPos({ 1887.0f ,540.0f });
		obbPlayer->SetPos({ 1937.0f ,540.0f });
	}
	else if (true == EnterLevel4)
	{
		ibbPlayer->SetPos({ 2277.0f ,540.0f });
		obbPlayer->SetPos({ 2327.0f ,540.0f });
	}

	LobbySettingInit();

	// LeveClear에 따른 Level 입장 상태 변경
	if (true == Level1ClearValue && false == Level2Open)
	{
		Level1Star->Activation();
		Level2Open = true;
		Level2Text->Activation();
		Level2Lock->Death();
		Level2Door->Activation();
	}
	if (true == Level2ClearValue && false == Level3Open)
	{
		Level2Star->Activation();
		Level3Open = true;
		Level3Text->Activation();
		Level3Lock->Death();
	}
	if (true == Level3ClearValue && false == Level4Open)
	{
		Level3Star->Activation();
		Level4Open = true;
		Level4Text->Activation();
		Level4Lock->Death();
	}
	if (true == Level4ClearValue)
	{
		Level4Star->Activation();
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
}

void Lobby::LevelPlayerInit()
{
	float DefaultPosX = 640.0f;
	float DefaultPosY = 300.0f;

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
	obbPlayer->SetPos({ DefaultPosX + 80.0f, DefaultPosY });
	obbPlayer->ChangeState(PlayerState::Fall);
	obbPlayer->SetDir(PlayerDir::Right);
}

void Lobby::Level1Start(float _DeltaTime)
{
	static float Time = 0.0f;
	Time += _DeltaTime;
	if (false == EndFadeInit)
	{
		LobbyEndFade = CreateActor<Fade>();
		LobbyEndFade->Init("FadeBlack.bmp", FadeState::FadeOut);
		LobbyEndFade->SetFadeSpeed(400.0f);

		EffectPlayer = GameEngineSound::SoundPlay("EnterLevel.mp3");
		EffectPlayer.SetVolume(0.5f);

		EndFadeInit = true;
	}

	if (LobbyEndFade == nullptr)
	{
		MsgBoxAssert("LobbyEndFade가 nullptr입니다.")
	}
	else
	{
		if (true == LobbyEndFade->FadeIsEnd())
		{
			Time = 0.0f;
			BGMPlayer.Stop();
			GameEngineCore::ChangeLevel("PlayLevel1");
		}
	}

	
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(Time + 1.0f);
}

void Lobby::Level2Start(float _DeltaTime)
{
	static float Time = 0.0f;
	Time += _DeltaTime;
	if (false == EndFadeInit)
	{
		LobbyEndFade = CreateActor<Fade>();
		LobbyEndFade->Init("FadeBlack.bmp", FadeState::FadeOut);
		LobbyEndFade->SetFadeSpeed(400.0f);

		EffectPlayer = GameEngineSound::SoundPlay("EnterLevel.mp3");
		EffectPlayer.SetVolume(0.5f);

		EndFadeInit = true;
	}

	if (LobbyEndFade == nullptr)
	{
		MsgBoxAssert("LobbyEndFade가 nullptr입니다.")
	}
	else
	{
		if (true == LobbyEndFade->FadeIsEnd())
		{
			Time = 0.0f;
			BGMPlayer.Stop();
			GameEngineCore::ChangeLevel("PlayLevel2");
		}
		GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(Time + 1.0f);
	}
	
	return;
}

void Lobby::Level3Start(float _DeltaTime)
{
	return;
}

void Lobby::Level4Start(float _DeltaTime)
{
	return;
}

void Lobby::LobbySettingInit()
{
	EnterLevel1 = false;
	EnterLevel2 = false;
	EnterLevel3 = false;
	EndFadeInit = false;
}