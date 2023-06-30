#include "PlayLevel1_1.h"

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



PlayLevel1_1::PlayLevel1_1()
{
	WindowScale = GameEngineWindow::MainWindow.GetScale();
}

PlayLevel1_1::~PlayLevel1_1()
{

}

void PlayLevel1_1::Start()
{

}

void PlayLevel1_1::Update(float _DeltaTime)
{
	if (ibbPlayer->GetOtherPlayer() == nullptr ||
		obbPlayer->GetOtherPlayer() == nullptr)
	{
		MsgBoxAssert("OtherPlayer가 세팅되지 않았습니다.")
	}

	if (true == GameEngineInput::IsDown('I'))
	{
		EXMap->SwitchRender();
		CollisionDebugRenderSwitch();
	}

	//// ZoomIn, ZoomOut
	//if (true == GameEngineInput::IsPress('Z'))
	//{
	//	GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.005f);
	//}

	//if (true == GameEngineInput::IsPress('X'))
	//{
	//	GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.005f);
	//	float ZoomScale = GameEngineWindow::MainWindow.GetDoubleBufferingCopyScaleRatio();
	//	if (1.0f > ZoomScale)
	//	{
	//		GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
	//	}
	//}

	float4 ibbPlayerPos = ibbPlayer->GetPos();
	float4 obbPlayerPos = obbPlayer->GetPos();
	float4 VectorFromibbToobb = obbPlayerPos - ibbPlayerPos;
	float DistanceXFromibbToobb = obbPlayerPos.X - ibbPlayerPos.X;
	float4 CameraPos;

	if (false == ibbPlayer->IsUpdate())
	{
		CameraPos = obbPlayer->GetPos() - WindowScale.Half();
	}
	else if (false == obbPlayer->IsUpdate())
	{
		CameraPos = ibbPlayer->GetPos() - WindowScale.Half();
	}
	else
	{
		CameraPos = (ibbPlayerPos + (VectorFromibbToobb) * 0.5f - WindowScale.Half());
	}

	float Level1_MapScaleX = 7956.0f;
	float4 WindowHalfX = WindowScale.Half();

	//float MinCameraPosX = -(ZoomScale - 1) * WindowHalfX.hX();
	if (CameraPos.X < 0.0f)
	{
		CameraPos.X = 0.0f;
	}
	else if (CameraPos.X > Level1_MapScaleX - WindowScale.X)
	{
		CameraPos.X = Level1_MapScaleX - WindowScale.X;
	}

	GetMainCamera()->SetPos(CameraPos);


	//if (0.0f > DistanceXFromibbToobb)
	//{
	//	DistanceXFromibbToobb = -DistanceXFromibbToobb;
	//}

	//float ZoomScale = 2.0f - (DistanceXFromibbToobb / (WindowScale.X - 100.0f));

	//if (2.0f < ZoomScale)
	//{
	//	ZoomScale = 2.0f;
	//}
	//else if (1.0f > ZoomScale)
	//{
	//	ZoomScale = 1.0f;
	//}

	float CameraPosX = GetMainCamera()->GetPos().X;

	if (500.0f > CameraPosX)
	{
		ZoomScale -= 0.5f * _DeltaTime;
		if (1.0f > ZoomScale)
		{
			ZoomScale = 1.0f;
		}
	}
	else if (3000.0f > CameraPosX)
	{
		ZoomScale += 0.5f * _DeltaTime;
		if (1.4f < ZoomScale)
		{
			ZoomScale = 1.4f;
		}
	}
	else if (4800.0f > CameraPosX)
	{
		ZoomScale -= 0.5f * _DeltaTime;
		if (1.0f > ZoomScale)
		{
			ZoomScale = 1.0f;
		}
	}
	else if (5300.0f > CameraPosX)
	{
		ZoomScale += 0.5f * _DeltaTime;
		if (1.4f < ZoomScale)
		{
			ZoomScale = 1.4f;
		}
	}
	else if (5900.0f < CameraPosX)
	{
		ZoomScale -= 0.5f * _DeltaTime;
		if (1.0f > ZoomScale)
		{
			ZoomScale = 1.0f;
		}
	}
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(ZoomScale);
	Back->SetPos(GetMainCamera()->GetPos() + WindowScale.Half());

	if (true == GameEngineInput::IsDown('N'))
	{
		GameEngineCore::ChangeLevel("Lobby");
	}
}

void PlayLevel1_1::Render(float _DeltaTime)
{
	
}

void PlayLevel1_1::LevelStart(GameEngineLevel* _PrevLevel)
{
	Back = CreateActor<BackGround>(RenderOrder::BackGround);
	Back->Init("OrangeGreen.bmp");

	EXMap = CreateActor<Map>();
	EXMap->Init("Level1-1.bmp", "Level1-1_Collision.bmp");

	// ibb
	float DefaultPosX = 300.0f;
	float DefaultPosY = 900.0f;
	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetGroundTexture("Level1-1_Collision.bmp");
	ibbPlayer->SetPos({ DefaultPosX, DefaultPosY });
	ibbPlayer->SetRatio(2.0f);

	// obb
	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetGroundTexture("Level1-1_Collision.bmp");
	obbPlayer->SetPos({ DefaultPosX + 100.0f, DefaultPosY });
	obbPlayer->SetRatio(2.0f);

	obbPlayer->SetOtherPlayer(dynamic_cast<Player*>(ibbPlayer));
	ibbPlayer->SetOtherPlayer(dynamic_cast<Player*>(obbPlayer));

	// RoadMonster
	{
		RoadMonster* _RoadMonster = nullptr;
		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_RoadMonster->ReverseInit();
		_RoadMonster->SetPos({ 3200.0f, 755.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_RoadMonster->ReverseInit();
		_RoadMonster->SetPos({ 3100.0f, 755.0f });;
		_RoadMonster->SetDir(RoadMonsterDir::Right);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->Init();
		_RoadMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_RoadMonster->SetPos({ 3400.0f, 649.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Right);
		RoadMonsters.push_back(_RoadMonster);

		_RoadMonster = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
		_RoadMonster->Init();
		_RoadMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_RoadMonster->SetPos({ 3600.0f, 649.0f });
		_RoadMonster->SetDir(RoadMonsterDir::Left);
		RoadMonsters.push_back(_RoadMonster);
	}

	// JumpingMonster

	{
		JumpingMonster* _JumpingMonster = nullptr;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ 2100.0f, 800.0f });
		_JumpingMonster->SetJumpForce(500.0f);
		_JumpingMonster->Init();
		JumpingMonsters.push_back(_JumpingMonster);

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ 2500.0f, 1100.0f });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		float DefaultPosX = 5800.0f;
		float DefaultPosY = 1200.0f;
		float UnitPosX = 70.0f;
		float UnitPosY = 100.0f;

		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
		_JumpingMonster->ReverseInit();
		JumpingMonsters.push_back(_JumpingMonster);

		DefaultPosX += UnitPosX;
		DefaultPosY += UnitPosY;
		_JumpingMonster = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
		_JumpingMonster->SetGroundTexture("Level1-1_Collision.bmp");
		_JumpingMonster->SetPos({ DefaultPosX, DefaultPosY });
		_JumpingMonster->SetJumpForce(600.0f);
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
		_Warp->SetPos({ 6872.0f, 1027.0f });
		_Warp->Init();
		Warps.push_back(_Warp);
	}
}

void PlayLevel1_1::LevelEnd(GameEngineLevel* _NextLevel)
{
	obbPlayer->OverOn();
	ibbPlayer->OverOn();

	for (int i = 0; i < RoadMonsters.size(); i++)
	{
		RoadMonsters[i]->OverOff();
	}

	for (int i = 0; i < JumpingMonsters.size(); i++)
	{
		JumpingMonsters[i]->OverOff();
	}

	for (int i = 0; i < Warps.size(); i++)
	{
		Warps[i]->OverOff();
	}
}