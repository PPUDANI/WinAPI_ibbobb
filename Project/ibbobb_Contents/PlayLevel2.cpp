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
#include "GravityTransferPlatform.h"

PlayLevel2::PlayLevel2()
{
}

PlayLevel2::~PlayLevel2()
{
}

void PlayLevel2::Start()
{
	LevelMaxScaleX = 7644.0f;

	// BackGround & Map 생성
	{
		Back = CreateActor<BackGround>(RenderOrder::BackGround);
		Back->Init("Level2_BackGround.bmp");

		LevelMap = CreateActor<Map>();
		LevelMap->Init("Level2_Map.bmp", ColName);
	}

	// Medal
	{
		Medal* _Medal = nullptr;
		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->Init();
		_Medal->SetPos({ 2800.0f, 550.0f });
		Medals.push_back(_Medal);

		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->Init();
		_Medal->SetPos({ 4017.0, 610.0f });
		Medals.push_back(_Medal);

		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->ReverseInit();
		_Medal->SetPos({ 5005.0f, 1300.0f });
		Medals.push_back(_Medal);
	}

	// Warp
	{
		// 1
		Warp* _Warp = nullptr;
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 1287.0f, 1096.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 1573.0f, 1096.0f });
		_Warp->Init(WarpType::obbOnly);
		Warps.push_back(_Warp);

		// 2
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 2327.0f, 1070.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 2509.0f, 889.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 2737.0f, 869.0f });
		_Warp->Init(WarpType::obbOnly);
		Warps.push_back(_Warp);

		// 3
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 3627.0f, 914.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		// 4
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 4453.0f, 1051.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetPos({ 4875.0f, 862.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		// 5
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 5988.0f, 1129.0f });
		_Warp->Init(WarpType::ibbOnly);
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(14);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetPos({ 6481.0f, 843.0f });
		_Warp->Init(WarpType::Common);
		Warps.push_back(_Warp);
	}

	// GravityPlatform
	{
		GravityTransferPlatform* _Platform = nullptr;
		_Platform = CreateActor<GravityTransferPlatform>(UpdateOrder::GravityPlatform);
		_Platform->SetPos({4017.0, 973.0f});
		GravityPlatforms.push_back(_Platform);


		_Platform = CreateActor<GravityTransferPlatform>(UpdateOrder::GravityPlatform);
		_Platform->SetPos({ 5005.0, 843.0f });
		GravityPlatforms.push_back(_Platform);

		_Platform = CreateActor<GravityTransferPlatform>(UpdateOrder::GravityPlatform);
		_Platform->SetPos({ 5241.0f, 843.0f });
		GravityPlatforms.push_back(_Platform);


		_Platform = CreateActor<GravityTransferPlatform>(UpdateOrder::GravityPlatform);
		_Platform->SetPos({ 6279.0, 1129.0f });
		GravityPlatforms.push_back(_Platform);
	}

	// Text
	{
		GoalText = CreateActor<DefaultImage>();
		GoalText->Init("GoalText.bmp");
		GoalText->LevitationOn();
		GoalText->SetPos({ 7448.0f, 680.0 });
		GoalText->SetSpeed(5.0f);
		GoalText->SetMovingHeight(13.0f);
	}

	LobyDoor = CreateActor<LevelDoor>();
	LobyDoor->SetPos({ 7448.0f, 801.0 });

	// Sound
	{
		SoundLoadManager::LoadSound("BGM", "Level2BGM.mp3");
	}
}

void PlayLevel2::Update(float _DeltaTime)
{
	// 최고 지점 좌표 저장
	if (GetMainCamera()->GetPos().X > StackCameraPosX)
	{
		StackCameraPosX = GetMainCamera()->GetPos().X;
	}

	GameOverCheck(_DeltaTime);

	if (true == GameEngineInput::IsDown('B'))
	{
		Lobby::Level1Clear();
		EnterLobby = true;
	}

	if (true == LobyDoor->IsPlayerEnter())
	{
		EnterLobby = true;
	}

	if (true == GameEngineInput::IsDown('1'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		
		ibbPlayer->SetPos({ 1000.0f, 1000.0f });
		obbPlayer->SetPos({ 1100.0f, 1000.0f });
	}

	if (true == GameEngineInput::IsDown('2'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 2385.0f, 900.0f });
		obbPlayer->SetPos({ 2455.0f, 900.0f });
	}

	if (true == GameEngineInput::IsDown('3'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 3550.0f, 1000.0f });
		obbPlayer->SetPos({ 3590.0f, 1000.0f });
	}

	if (true == GameEngineInput::IsDown('4'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 4730.0f, 950.0f });
		obbPlayer->SetPos({ 4790.0f, 950.0f });
	}

	if (true == GameEngineInput::IsDown('5'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 5800.0f, 1000.0f });
		obbPlayer->SetPos({ 5880.0f, 1000.0f });
	}

	if (true == GameEngineInput::IsDown('6'))
	{
		if (false == ibbPlayer->IsUpdate() ||
			false == obbPlayer->IsUpdate())
		{
			PlayLevelBGM();
		}

		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 7200.0f, 600.0f });
		obbPlayer->SetPos({ 7280.0f, 600.0f });
	}

	SubLevel::Update(_DeltaTime);
	float CameraPosX = GetMainCamera()->GetPos().X;

	if (300.0f > CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}
	else if (1300.0f > CameraPosX)
	{
		SetZoomScale(1.4f, _DeltaTime);
	}
	else if (7644.0f > CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}

	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(ZoomScale);

	if (false == ibbPlayer->IsUpdate() ||
		false == obbPlayer->IsUpdate())
	{
		BGMPlayer.Stop();
	}

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
		Lobby::Level1Clear(); // 임시
		Lobby::Level2Clear();
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
		// 1
		RoadMonster* _RoadMonster = nullptr;
		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->Init({ 1650.0f, 1219.0f }, RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->Init({ 1950.0f, 1219.0f }, RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->Init({ 2200.0f, 1219.0f }, RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);
	}

	// JumpingMonster
	{
		// 2
		JumpingMonster* _JumpingMonster = nullptr;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->Init({ 3200.0f, 1050.0f });
		JumpingMonsters.push_back(_JumpingMonster);

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->Init({ 3330.0f, 850.0f });
		JumpingMonsters.push_back(_JumpingMonster);

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->Init({ 3460.0f, 1050.0f });
		JumpingMonsters.push_back(_JumpingMonster);

		// 5
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit({ 5590.0f, 1100.0f });
		JumpingMonsters.push_back(_JumpingMonster);
	}

	Level2StartFade = CreateActor<Fade>();
	Level2StartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
	Level2StartFade->SetFadeSpeed(400.0f);
}

void PlayLevel2::LevelEnd(GameEngineLevel* _NextLevel)
{
	ibbPlayer->OverOn();
	obbPlayer->OverOn();

	for (int i = 0; i < RoadMonsters.size(); i++)
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
	JumpingMonsters.clear();

	Level2StartFade->Death();
	Level2StartFade = nullptr;
	Level2EndFade->Death();
	Level2EndFade = nullptr;
	GetMainCamera()->SetPos(float4::ZERO);
	StackCameraPosX = 0.0f;
	Level2SettingInit();
}

void PlayLevel2::LevelPlayerInit()
{
	float DefaultPosX = 300.0f;
	float DefaultPosY = 800.0f;
	
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
	ibbPlayer->SetMaxGravityInHole(1100.0f);

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
	obbPlayer->SetMaxGravityInHole(1100.0f);
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

void PlayLevel2::PlayLevelBGM()
{
	BGMPlayer = GameEngineSound::SoundPlay("Level2BGM.mp3");
	BGMPlayer.SetLoop(10);
	BGMPlayer.SetVolume(0.3f);
}

void PlayLevel2::Level2SettingInit()
{
	EndFadeInit = false;
	EnterLobby = false;
}

void PlayLevel2::GameOverCheck(float _DeltaTime)
{
	static float ServiveTime = 0.0f;
	static bool KillMonster = false;
	if (false == ibbPlayer->IsUpdate() &&
		false == obbPlayer->IsUpdate())
	{
		if (false == KillMonster)
		{
			KillMonster = true;
			KillAllMonster();
		}

		if (ServiveTime >= 1.0f)
		{
			KillMonster = false;
			ServiveTime = 0.0f;
			ReviveCharacter();
			CharacterSpawn();
			SummonMonster();
			PlayLevelBGM();
		}
		else
		{
			ServiveTime += _DeltaTime;
		}
	}
}

void PlayLevel2::KillAllMonster()
{
	for (int i = 0; i < RoadMonsters.size(); i++)
	{
		if (RoadMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 RoadMonster를 Release 하려 했습니다.")
		}

		RoadMonsters[i]->SetAnimation("Dead");
		RoadMonsters[i]->SetCoreAnimation("Dead");
		RoadMonsters[i]->ChangeState(RoadMonsterState::Dead);
	}

	for (int i = 0; i < JumpingMonsters.size(); i++)
	{
		if (JumpingMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 JumpingMonster를 Release 하려 했습니다.")
		}

		JumpingMonsters[i]->SetAnimation("Dead");
		JumpingMonsters[i]->SetCoreAnimation("Dead");
		JumpingMonsters[i]->ChangeState(JumpingMonsterState::Dead);
	}
	EffectPlayer = GameEngineSound::SoundPlay("MonsterDeath.mp3");
}

void PlayLevel2::SummonMonster()
{
	for (int i = 0; i < RoadMonsters.size(); i++)
	{
		if (RoadMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 RoadMonster를 Release 하려 했습니다.")
		}
		RoadMonsters[i]->On();
		RoadMonsters[i]->SetAnimation("Live");
		RoadMonsters[i]->SetCoreAnimation("Live");
		RoadMonsters[i]->ChangeState(RoadMonsterState::Live);
	}

	for (int i = 0; i < JumpingMonsters.size(); i++)
	{
		if (JumpingMonsters[i] == nullptr)
		{
			MsgBoxAssert("null인 JumpingMonster를 Release 하려 했습니다.")
		}
		JumpingMonsters[i]->On();
		JumpingMonsters[i]->SetAnimation("Live");
		JumpingMonsters[i]->SetCoreAnimation("Live");
		JumpingMonsters[i]->ChangeState(JumpingMonsterState::Live);
	}
}

void PlayLevel2::CharacterSpawn()
{
	if (1600.0f > StackCameraPosX)
	{
		ibbPlayer->SetPos({ 1150.0f, 1000.0f });
		obbPlayer->SetPos({ 1230.0f, 1000.0f });
	}
	else if (2800.0f > StackCameraPosX)
	{
		ibbPlayer->SetPos({ 2385.0f, 900.0f });
		obbPlayer->SetPos({ 2455.0f, 900.0f });
	}
	else if (3550.0f > StackCameraPosX)
	{
		ibbPlayer->SetPos({ 3550.0f, 1000.0f });
		obbPlayer->SetPos({ 3590.0f, 1000.0f });
	}
	else if (5050.0f > StackCameraPosX)
	{
		ibbPlayer->SetPos({ 4330.0f, 950.0f });
		obbPlayer->SetPos({ 4390.0f, 950.0f });
	}
	else if (5750.0f > StackCameraPosX)
	{
		ibbPlayer->SetPos({ 5800.0f, 1000.0f });
		obbPlayer->SetPos({ 5880.0f, 1000.0f });
	}
	else if (6000.0f <= StackCameraPosX)
	{
		ibbPlayer->SetPos({ 6750.0f, 700.0f });
		obbPlayer->SetPos({ 6830.0f, 700.0f });
	}
}
