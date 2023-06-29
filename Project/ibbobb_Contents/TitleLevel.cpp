#include "TitleLevel.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineCore.h>

#include "ContentsEnum.h"
#include "BackGround.h"
#include "BlinkImage.h"

#include "Titleibb.h"
#include "Titleobb.h"

TitleLevel::TitleLevel()
{

}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::Start()
{
	
	Title = CreateActor<BackGround>(RenderOrder::BackGround);
	Title->Init("Title_BackGround.bmp");

	PressText = CreateActor<BlinkImage>(RenderOrder::Image);
	PressText->Init("Press_Text.bmp");
	PressText->SetPos({750.0f, 360.0f});
}

void TitleLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("PlayLevel");
	}
}

void TitleLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	// ibb
	TitleibbPlayer = CreateActor<Titleibb>(UpdateOrder::Player);
	TitleibbPlayer->SetPos({ 500.0f, 650.0f });

	// obb
	TitleobbPlayer = CreateActor<Titleobb>(UpdateOrder::Player);
	TitleobbPlayer->SetPos({ 1000.0f, 650.0f });

}

void TitleLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	TitleibbPlayer->OverOff();
	TitleobbPlayer->OverOff();
	PressText->OverOff();
	Title->OverOff();
}