#include "PlayLevel.h"

// Contents
#include "Player.h"
#include "BackGround.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCamera.h>
PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start() 
{
	Back = CreateActor<BackGround>();
	Back->Init("dd.Bmp", 1.0f);

	BackGround* Map = CreateActor<BackGround>();
	Map->Init("StageTest.Bmp", 2.0f);
	
	LevelPlayer = CreateActor<Player>();
}

void PlayLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}

	//Back->SetPos(LevelPlayer->GetPos());
}

void PlayLevel::Render() 
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