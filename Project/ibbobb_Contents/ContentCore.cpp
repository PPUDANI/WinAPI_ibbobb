#include "ContentCore.h"

#include <GameEnginePlatform/GameEngineWindow.h>

#include "TitleLevel.h"
#include "PlayLevel.h"
#include "EndingLevel.h"

ContentCore::ContentCore()
{

}

ContentCore::~ContentCore()
{

}


void ContentCore::Start()
{
	GameEngineWindow::MainWindow.SetPosAndScale({ 100, 50 }, { 1500, 900 });

	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");

	//GameEngineCore::ChangeLevel("TitleLevel");
	GameEngineCore::ChangeLevel("PlayLevel");
}

void ContentCore::Update(float _DeltaTime)
{

}

void ContentCore::Render(float _DeltaTime)
{

}

void ContentCore::Release()
{

}