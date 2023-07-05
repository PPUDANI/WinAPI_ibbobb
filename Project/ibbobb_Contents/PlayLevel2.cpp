#include "PlayLevel2.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCore.h>
#include <cmath>

#include "ContentsEnum.h"
#include "SoundLoadManager.h"
#include "BackGround.h"
#include "Map.h"
#include "RoadMonster.h"
#include "JumpingMonster.h"
#include "Warp.h"
#include "ibb.h"
#include "obb.h"
#include "Medal.h"
#include "DefaultImage.h"
#include "LevelDoor.h"
#include "Fade.h"
#include "Lobby.h"

PlayLevel2::PlayLevel2()
{
}

PlayLevel2::~PlayLevel2()
{
}

void PlayLevel2::Start()
{
	LevelMaxScaleX = 7956.0f;

	// BackGround & Map 생성
	{
		Back = CreateActor<BackGround>(RenderOrder::BackGround);
		Back->Init("Level2_BackGround.bmp");

		LevelMap = CreateActor<Map>();
		LevelMap->Init("Level2_Map.bmp", ColName);
	}

	// Medal
	{
		//Medal* _Medal = nullptr;
		//_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		//_Medal->Init();
		//_Medal->SetPos({ 1000.0f, 680.0f });
		//Medals.push_back(_Medal);

	}

	// Warp
	{
		// 1
		Warp* _Warp = nullptr;
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 1287.0f, 864.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 1573.0f, 864.0f });
		_Warp->Init(WarpType::obbOnly);
		Warps.push_back(_Warp);

		// 2
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 2327.0f, 838.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 2509.0f, 655.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 2737.0f, 637.0f });
		_Warp->Init(WarpType::obbOnly);
		Warps.push_back(_Warp);

		// 3
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 3627.0f, 682.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		// 4
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 4609.0f, 923.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 6143.0f, 1027.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 6637.0f, 559.0f });
		_Warp->Init(WarpType::Common);
		Warps.push_back(_Warp);
	}

	//// Text
	//{
	//	GoalText = CreateActor<DefaultImage>();
	//	GoalText->Init("GoalText.bmp");
	//	GoalText->LevitationOn();
	//	GoalText->SetPos({ 7813.0f, 865.0f });
	//	GoalText->SetSpeed(5.0f);
	//	GoalText->SetMovingHeight(13.0f);
	//}

	LobyDoor = CreateActor<LevelDoor>();
	LobyDoor->SetPos({ 2500.0f, 960.0f });

	// Sound
	{
		if (nullptr == GameEngineSound::FindSound("Level2BGM.mp3"))
		{
			SoundLoadManager::LoadSound("BGM", "Level2BGM.mp3");
		}

		if (nullptr == GameEngineSound::FindSound("EnterLevel.mp3"))
		{
			SoundLoadManager::LoadSound("LevelEffect", "EnterLevel.mp3");
		}
	}
}

void PlayLevel2::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('B'))
	{
		Lobby::Level1Clear();
		EnterLobby = true;
	}

	if (true == LobyDoor->IsPlayerEnter())
	{
		EnterLobby = true;
	}

	//if (true == GameEngineInput::IsDown('1'))
	//{
	//	if (false == ibbPlayer->IsUpdate() &&
	//		false == obbPlayer->IsUpdate())
	//	{
	//		ReviveCharacter();
	//	}

	//	ibbPlayer->SetPos({ 1650.0f, 500.0f });
	//	obbPlayer->SetPos({ 1720.0f, 500.0f });
	//}

	//if (true == GameEngineInput::IsDown('2'))
	//{
	//	if (false == ibbPlayer->IsUpdate() &&
	//		false == obbPlayer->IsUpdate())
	//	{
	//		ReviveCharacter();
	//	}
	//	ibbPlayer->SetPos({ 3000.0f, 200.0f });
	//	obbPlayer->SetPos({ 3100.0f, 200.0f });
	//}

	//if (true == GameEngineInput::IsDown('3'))
	//{
	//	if (false == ibbPlayer->IsUpdate() &&
	//		false == obbPlayer->IsUpdate())
	//	{
	//		ReviveCharacter();
	//	}
	//	ibbPlayer->SetPos({ 4000.0f, 500.0f });
	//	obbPlayer->SetPos({ 4100.0f, 500.0f });
	//}

	//if (true == GameEngineInput::IsDown('4'))
	//{
	//	if (false == ibbPlayer->IsUpdate() &&
	//		false == obbPlayer->IsUpdate())
	//	{
	//		ReviveCharacter();
	//	}
	//	ibbPlayer->SetPos({ 5550.0f, 500.0f });
	//	obbPlayer->SetPos({ 5650.0f, 500.0f });
	//}

	SubLevel::Update(_DeltaTime);
	float CameraPosX = GetMainCamera()->GetPos().X;

	//if (500.0f > CameraPosX)
	//{
	//	SetZoomScale(1.0f, _DeltaTime);
	//}
	//else if (3000.0f > CameraPosX)
	//{
	//	SetZoomScale(1.4f, _DeltaTime);
	//}
	//else if (4800.0f > CameraPosX)
	//{
	//	SetZoomScale(1.0f, _DeltaTime);
	//}
	//else if (5300.0f > CameraPosX)
	//{
	//	SetZoomScale(1.4f, _DeltaTime);
	//}
	if (7000.0f > CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}

	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(ZoomScale);

	if (true == EnterLobby)
	{
		LobbyStart(_DeltaTime);
	}
}

void PlayLevel2::Render(float _DeltaTime)
{

}

void PlayLevel2::LobbyStart(float _DeltaTime)
{
	if (false == EndFadeInit)
	{
		Level2EndFade = CreateActor<Fade>();
		Level2EndFade->Init("FadeBlack.bmp", FadeState::FadeOut);
		Level2EndFade->SetFadeSpeed(400.0f);

		EffectPlayer = GameEngineSound::SoundPlay("EnterLevel.mp3");
		EffectPlayer.SetVolume(0.5f);

		EndFadeInit = true;
	}

	if (true == Level2EndFade->FadeIsEnd())
	{
		Lobby::Level2Clear();
		BGMPlayer.Stop();
		GameEngineCore::ChangeLevel("Lobby");
	}
}



void PlayLevel2::LevelStart(GameEngineLevel* _PrevLevel)
{
	LevelPlayerInit();

	{
		BGMPlayer = GameEngineSound::SoundPlay("Level2BGM.mp3");
		BGMPlayer.SetLoop(10);
		BGMPlayer.SetVolume(0.3f);
	}

	// RoadMonster
	{
		
	}

	// JumpingMonster
	{
		
	}

	Level2StartFade = CreateActor<Fade>();
	Level2StartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
	Level2StartFade->SetFadeSpeed(400.0f);
}

void PlayLevel2::LevelEnd(GameEngineLevel* _NextLevel)
{
	ibbPlayer->OverOn();
	obbPlayer->OverOn();

	/*for (int i = 0; i < RoadMonsters.size(); i++)
	{
		if (RoadMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 RoadMonster를 Release 하려 했습니다.")
		}
		RoadMonsters[i]->Death();
		RoadMonsters[i] = nullptr;
	}
	RoadMonsters.clear();

	for (int i = 0; i < JumpingMonsters.size(); i++)
	{
		if (JumpingMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 JumpingMonster를 Release 하려 했습니다.")
		}
		JumpingMonsters[i]->Death();
		JumpingMonsters[i] = nullptr;
	}
	JumpingMonsters.clear();*/

	Level2StartFade->Death();
	Level2StartFade = nullptr;
	Level2EndFade->Death();
	Level2EndFade = nullptr;

	Level2SettingInit();
}

void PlayLevel2::LevelPlayerInit()
{
	float DefaultPosX = 4000.0f;
	float DefaultPosY = 380.0f;

	// ibb
	if (nullptr == ibb::GetMainibb())
	{
		MsgBoxAssert("이전 레벨에서 ibb플레이어가 세팅되지 않았습니다.")
	}
	ibbPlayer = ibb::GetMainibb();
	ibbPlayer->SetGroundTexture(ColName);
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });
	ibbPlayer->ChangeState(PlayerState::Fall);
	ibbPlayer->SetDir(PlayerDir::Right);
	ibbPlayer->SetMaxGravityInHole(1300.0f);

	// obb
	if (nullptr == obb::GetMainobb())
	{
		MsgBoxAssert("이전 레벨에서 obb플레이어가 세팅되지 않았습니다.")
	}
	obbPlayer = obb::GetMainobb();
	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });
	obbPlayer->ChangeState(PlayerState::Fall);
	obbPlayer->SetDir(PlayerDir::Right);
	obbPlayer->SetMaxGravityInHole(1200.0f);
}

void PlayLevel2::SetZoomScale(float _Ratio, float _DeltaTime)
{
	if (_Ratio >= ZoomScale)
	{
		ZoomScale += 0.5f * _DeltaTime;
		if (_Ratio < ZoomScale)
		{
			ZoomScale = _Ratio;
		}
	}
	else if (_Ratio < ZoomScale)
	{
		ZoomScale -= 0.5f * _DeltaTime;
		if (_Ratio > ZoomScale)
		{
			ZoomScale = _Ratio;
		}
	}

}

void PlayLevel2::Level2SettingInit()
{
	EndFadeInit = false;
	EnterLobby = false;
}