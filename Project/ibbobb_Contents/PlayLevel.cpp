#include "PlayLevel.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include "Player.h"
#include "BackGround.h"
#include "Map.h"
#include "ContentsEnum.h"

PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start() 
{
	UpBack = CreateActor<BackGround>(UpdateOrder::BackGround);
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
	EXMap->Init("EXLevel.bmp","EXLevel_Collision.bmp");
	LevelPlayer = CreateActor<Player>(UpdateOrder::Player);
	LevelPlayer->SetGroundTexture("EXLevel_Collision.bmp");

	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.5f);
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
	UpBack->SetPos(LevelPlayer->GetPos());
}

void PlayLevel::Release() 
{

}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr == LevelPlayer)
	{
		MsgBoxAssert("플레이어를 세팅해주지 않았습니다");
	}
}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}