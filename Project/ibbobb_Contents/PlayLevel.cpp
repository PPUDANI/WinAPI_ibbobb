#include "PlayLevel.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCamera.h>

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
	Back = CreateActor<BackGround>(UpdateOrder::BackGround);
	Back->Init("RedSky.bmp");

	EXMap = CreateActor<Map>();
	EXMap->Init("EXMap.bmp","EXMapCollision.bmp");
	LevelPlayer = CreateActor<Player>(UpdateOrder::Player);
	LevelPlayer->SetGroundTexture("EXMapCollision.bmp");
}

void PlayLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		EXMap->SwitchRender();
	}

	Back->SetPos(LevelPlayer->GetPos());
}

void PlayLevel::Render(float _DeltaTime)
{
	Back->SetPos(LevelPlayer->GetPos());
}

void PlayLevel::Release() 
{

}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr == LevelPlayer)
	{
		MsgBoxAssert("�÷��̾ ���������� �ʾҽ��ϴ�");
	}
}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}