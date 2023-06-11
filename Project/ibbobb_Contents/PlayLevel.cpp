#include "PlayLevel.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

#include "BackGround.h"
#include "Map.h"
#include "Player.h"
#include "ibb.h"
#include "obb.h"
#include "RoadMonster.h"
#include "JumpingMonster.h"
#include "ContentsEnum.h"

PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start()
{
	UpBack = CreateActor<BackGround>(RenderOrder::BackGround);
	UpBack->Init("BlueSky.bmp");

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

	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetGroundTexture("EXLevel_Collision.bmp");

	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetGroundTexture("EXLevel_Collision.bmp");




	RoadMonster0 = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
	RoadMonster0->SetGroundTexture("EXLevel_Collision.bmp");

	JumpingMonster0 = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
	JumpingMonster0->SetGroundTexture("EXLevel_Collision.bmp");

	//GetMainCamera()->SetPos({-50, 300});
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
}

void PlayLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		EXMap->SwitchRender();
	}

}

void PlayLevel::Render(float _DeltaTime)
{
	//UpBack->SetPos(obbPlayer->GetPos());
}

void PlayLevel::Release() 
{

}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	//if (nullptr == ibbPlayer)
	//{
	//	MsgBoxAssert("플레이어를 세팅해주지 않았습니다");
	//}
	//if (nullptr == obbPlayer)
	//{
	//	MsgBoxAssert("플레이어를 세팅해주지 않았습니다");
	//}
}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}