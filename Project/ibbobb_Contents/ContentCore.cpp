#include "ContentCore.h"

#include "TitleLevel.h"
#include "PlayLevel.h"
#include "EndingLevel.h"
#include <GameEnginePlatform/GameEngineWindow.h>
ContentCore::ContentCore()
{

}

ContentCore::~ContentCore()
{

}


void ContentCore::Start()
{
	GameEngineWindow::MainWindow.SetPosAndScale({ 200, 100 }, { 1400, 800 });

	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");

	//GameEngineCore::ChangeLevel("TitleLevel");
	GameEngineCore::ChangeLevel("TitleLevel");
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