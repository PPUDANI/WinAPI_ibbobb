#include "PlayLevel.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCore.h>

#include "ContentsEnum.h"
#include "BackGround.h"
#include "Map.h"
#include "RoadMonster.h"
#include "JumpingMonster.h"
#include "ibb.h"
#include "obb.h"



PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start()
{
	UpBack = CreateActor<BackGround>(RenderOrder::BackGround);
	UpBack->Init("OrangeGreen.bmp");

	//EXMap = CreateActor<Map>();
	//EXMap->Init("Level1_Map.bmp","Level1_MapCollision.bmp");
	//LevelPlayer = CreateActor<Player>(UpdateOrder::Player);
	//LevelPlayer->SetGroundTexture("Level1_MapCollision.bmp");

	//EXMap = CreateActor<Map>();
	//EXMap->Init("EXMap.bmp","EXMapCollision.bmp");
	//LevelPlayer = CreateActor<Player>(UpdateOrder::Player);
	//LevelPlayer->SetGroundTexture("EXMapCollision.bmp");

	EXMap = CreateActor<Map>();
	EXMap->Init("EXLevel.bmp", "EXLevel_Collision.bmp");


	//GetMainCamera()->SetPos({-50, 300});

	
}

void PlayLevel::Update(float _DeltaTime)
{
	if (ibbPlayer->GetOtherPlayer() == nullptr ||
		obbPlayer->GetOtherPlayer() == nullptr)
	{
		MsgBoxAssert("OtherPlayer가 세팅되지 않았습니다.")
	}

	if (true == GameEngineInput::IsDown('O'))
	{
		EXMap->SwitchRender();
		CollisionDebugRenderSwitch();
	}

	// ZoomIn, ZoomOut
	if (true == GameEngineInput::IsPress('Z'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.005f);
	}

	if (true == GameEngineInput::IsPress('X'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.005f);
		float ZoomScale = GameEngineWindow::MainWindow.GetDoubleBufferingCopyScaleRatio();
		if (1.0f > ZoomScale)
		{
			GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
		}
	}
}

void PlayLevel::Render(float _DeltaTime)
{
	float4 WindowScaleHlaf = { 750.0f, 450.0f };
	UpBack->SetPos(GetMainCamera()->GetPos() + WindowScaleHlaf);
}

void PlayLevel::Release() 
{

}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	// ibb
	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetGroundTexture("EXLevel_Collision.bmp");
	ibbPlayer->SetPos({ 1000.0f, 500.0f });
	ibbPlayer->SetRatio(2.0f);

	// obb
	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetGroundTexture("EXLevel_Collision.bmp");
	obbPlayer->SetPos({ 1100.0f, 500.0f });
	obbPlayer->SetRatio(2.0f);

	obbPlayer->SetOtherPlayer(dynamic_cast<Player*>(ibbPlayer));
	ibbPlayer->SetOtherPlayer(dynamic_cast<Player*>(obbPlayer));

	// Monster
	RoadMonster0 = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
	RoadMonster0->SetGroundTexture("EXLevel_Collision.bmp");
	RoadMonster0->Init();
	RoadMonster0->SetPos({ 2416.0f, 727.0f });
	RoadMonster0->SetDir(RoadMonsterDir::Left);
	RoadMonster0->SetMovementDistance(250.0f);
	RoadMonster0->SetSpeed(100.0f);

	RoadMonster1 = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
	RoadMonster1->ReverseInit();
	RoadMonster1->SetGroundTexture("EXLevel_Collision.bmp");
	RoadMonster1->SetPos({ 3000.0f, 677.0f });
	RoadMonster1->SetDir(RoadMonsterDir::Left);
	RoadMonster1->SetMovementDistance(500.0f);
	RoadMonster1->SetSpeed(100.0f);
	
	JumpingMonster0 = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
	JumpingMonster0->SetGroundTexture("EXLevel_Collision.bmp");
	JumpingMonster0->SetPos({ 1300.0f, 800.0f });
	JumpingMonster0->SetJumpForce(500.0f);
}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	obbPlayer->OverOn();
	ibbPlayer->OverOn();
	RoadMonster0->OverOff();
	RoadMonster1->OverOff();
	JumpingMonster0->OverOff();
}