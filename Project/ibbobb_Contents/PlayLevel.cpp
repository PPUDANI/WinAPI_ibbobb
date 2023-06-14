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

	// ibb
	ibbPlayer = CreateActor<ibb>(UpdateOrder::Player);
	ibbPlayer->SetGroundTexture("EXLevel_Collision.bmp");
	ibbPlayer->SetPos({ 1200, 600 });

	// obb
	obbPlayer = CreateActor<obb>(UpdateOrder::Player);
	obbPlayer->SetGroundTexture("EXLevel_Collision.bmp");
	obbPlayer->SetPos({ 1200, 600 });

	// Monster
	RoadMonster0 = CreateActor<RoadMonster>(UpdateOrder::RoadMonster);
	RoadMonster0->SetGroundTexture("EXLevel_Collision.bmp");
	RoadMonster0->SetPos({ 700.0f, 787.0f }); //  709.0f
	RoadMonster0->SetMovementDistance(200.0f);

	JumpingMonster0 = CreateActor<JumpingMonster>(UpdateOrder::JumpingMonster);
	JumpingMonster0->SetGroundTexture("EXLevel_Collision.bmp");
	JumpingMonster0->SetPos({ 1400.0f, 400.0f });
	//GetMainCamera()->SetPos({-50, 300});

}

void PlayLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		EXMap->SwitchRender();
		CollisionDebugRenderSwitch();
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