#include "PlayLevel1.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCore.h>
#include <cmath>

#include "ContentsEnum.h"
#include "BackGround.h"
#include "Map.h"
#include "RoadMonster.h"
#include "JumpingMonster.h"
#include "Warp.h"
#include "ibb.h"
#include "obb.h"
#include "Medal.h"
#include "Fade.h"


PlayLevel1::PlayLevel1()
{
	
}

PlayLevel1::~PlayLevel1()
{

}

void PlayLevel1::Start()
{
	// Medal
	{
		Medal* _Medal = nullptr;
		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->Init();
		_Medal->SetPos({ 1000.0f, 680.0f });
		Medals.push_back(_Medal);

		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->Init();
		_Medal->SetPos({ 5057.0f, 490.0f });
		Medals.push_back(_Medal);

		_Medal = CreateActor<Medal>(UpdateOrder::Medal);
		_Medal->Init();
		_Medal->SetPos({ 6800.0f, 500.0f });
		Medals.push_back(_Medal);
	}

}

void PlayLevel1::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('B'))
	{
		LobbyOn = true;
	}

	if (true == GameEngineInput::IsDown('G'))
	{
		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		
		ibbPlayer->SetPos({ 1000.0f, 500.0f});
		obbPlayer->SetPos({ 1100.0f, 500.0f});
	}
	
	if (true == GameEngineInput::IsDown('H'))
	{
		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 3000.0f, 200.0f });
		obbPlayer->SetPos({ 3100.0f, 200.0f });
	}
	
	if (true == GameEngineInput::IsDown('J'))
	{
		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 4000.0f, 500.0f });
		obbPlayer->SetPos({ 4100.0f, 500.0f });
	}

	if(true == GameEngineInput::IsDown('K'))
	{
		if (false == ibbPlayer->IsUpdate() &&
			false == obbPlayer->IsUpdate())
		{
			ReviveCharacter();
		}
		ibbPlayer->SetPos({ 7100.0f, 400.0f });
		obbPlayer->SetPos({ 7200.0f, 400.0f });
	}

	SubLevel::Update(_DeltaTime);
	float CameraPosX = GetMainCamera()->GetPos().X;

	if (500.0f > CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}
	else if (3000.0f > CameraPosX)
	{
		SetZoomScale(1.4f, _DeltaTime);
	}
	else if (4800.0f > CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}
	else if (5300.0f > CameraPosX)
	{
		SetZoomScale(1.4f, _DeltaTime);
	}
	else if (5900.0f < CameraPosX)
	{
		SetZoomScale(1.0f, _DeltaTime);
	}

	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(ZoomScale);
	
	if (true == LobbyOn)
	{
		LobbyStart(_DeltaTime);
	}
}

void PlayLevel1::Render(float _DeltaTime)
{
	
}

void PlayLevel1::LobbyStart(float _DeltaTime)
{
	if (false == EndFadeInit)
	{
		Level1EndFade = CreateActor<Fade>();
		Level1EndFade->Init("FadeBlack.bmp", FadeState::FadeOut);
		Level1EndFade->SetFadeSpeed(400.0f);

		EndFadeInit = true;
	}

	if (true == Level1EndFade->FadeIsEnd())
	{
		GameEngineCore::ChangeLevel("Lobby");
	}
}



void PlayLevel1::LevelStart(GameEngineLevel* _PrevLevel)
{
	Back = CreateActor<BackGround>(RenderOrder::BackGround);
	Back->Init("Level1_BackGround.bmp");

	LevelMap = CreateActor<Map>();
	LevelMap->Init("Level1_Map.bmp", ColName);
	LevelMaxScaleX = 7956.0f;

	LevelPlayerInit();

	// RoadMonster
	{
		RoadMonster* _RoadMonster = nullptr;
		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->ReverseInit();
		_RoadMonster->SetPos({ 3200.0f, 755.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->ReverseInit();
		_RoadMonster->SetPos({ 3100.0f, 755.0f });;
		_RoadMonster->SetDir(RoadMonsterDir::Right);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->Init();
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->SetPos({ 3400.0f, 649.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Right);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->Init();
		_RoadMonster->SetGroundTexture(ColName);
		_RoadMonster->SetPos({ 3600.0f, 649.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);
	}

	// JumpingMonster

	{
		JumpingMonster* _JumpingMonster = nullptr;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ 2100.0f, 800.0f });
		_JumpingMonster->SetJumpForce(500.0f);
		_JumpingMonster->Init();
		JumpingMonsters.push_back(_JumpingMonster);

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ 2500.0f, 1100.0f });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		float DefaultPosX = 5800.0f;
		float DefaultPosY = 1200.0f;
		float UnitPosX = 70.0f;
		float UnitPosY = 100.0f;
		float Jumpforce = 600.0f;

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture(ColName);
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(Jumpforce);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);
	}


	// Warp
	{
		Warp* _Warp = nullptr;
		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 1774.0f, 897.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 2769.0f, 760.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 3887.0f, 682.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 4791.0f, 975.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 4980.0f, 578.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(10);
		_Warp->SetWorpDir(WarpDir::Vertical);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 5526.0f, 1124.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 6585.0f, 1105.0f });
		_Warp->Init();
		Warps.push_back(_Warp);

		_Warp = CreateActor<Warp>(UpdateOrder::Warp);
		_Warp->SetStarNumber(12);
		_Warp->SetWorpDir(WarpDir::Horizontal);
		_Warp->SetWorpType(WarpType::Common);
		_Warp->SetPos({ 6871.0f, 1027.0f });
		_Warp->Init();
		Warps.push_back(_Warp);
	}

	Level1StartFade = CreateActor<Fade>();
	Level1StartFade->Init("FadeBlack.bmp", FadeState::FadeIn);
	Level1StartFade->SetFadeSpeed(400.0f);
}

void PlayLevel1::LevelEnd(GameEngineLevel* _NextLevel)
{
	Back->Death();
	LevelMap->Death();

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

	for (int i = 0; i < Warps.size(); i++)
	{
		if (Warps[i] == nullptr)
		{
			MsgBoxAssert("null인 Warp를 Release 하려 했습니다.")
		}
		Warps[i]->Death();
		Warps[i] = nullptr;
	}
	Warps.clear();

	Level1StartFade->Death();
	Level1StartFade = nullptr;

	Level1EndFade->Death();
	Level1EndFade = nullptr;

	Level1SettingInit();
}

void PlayLevel1::LevelPlayerInit()
{
	float DefaultPosX = 200.0f;
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
	ibbPlayer->SetDir(PlayerDir::Left);

	// obb
	if (nullptr == obb::GetMainobb())
	{
		MsgBoxAssert("이전 레벨에서 obb플레이어가 세팅되지 않았습니다.")
	}
	obbPlayer = obb::GetMainobb();
	obbPlayer->SetGroundTexture(ColName);
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });
	obbPlayer->ChangeState(PlayerState::Fall);
	ibbPlayer->SetDir(PlayerDir::Left);
}

void PlayLevel1::SetZoomScale(float _Ratio, float _DeltaTime)
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

void PlayLevel1::Level1SettingInit()
{
	EndFadeInit = false;
	LobbyOn = false;
}