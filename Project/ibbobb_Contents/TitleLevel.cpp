#include "TitleLevel.h"
#include "BackGround.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEnginePlatform/GameEngineWindow.h>
TitleLevel::TitleLevel()
{

}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::Start()
{
	BackGround* TitleBack = CreateActor<BackGround>();
	TitleBack->Init("Title.bmp");
	GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
}

void TitleLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("PlayLevel");
	}
}
