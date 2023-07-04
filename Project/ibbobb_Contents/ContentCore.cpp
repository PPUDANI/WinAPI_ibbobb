#include "ContentCore.h"

#include <GameEnginePlatform/GameEngineWindow.h>

#include "TitleLevel.h"
#include "Lobby.h"
#include "PlayLevel1.h"
#include "PlayLevel2.h"
#include "SubLevel.h"
ContentCore::ContentCore()
{

}

ContentCore::~ContentCore()
{

}


void ContentCore::Start()
{
	GameEngineWindow::MainWindow.SetPosAndScale({ 100, 50 }, { 1500, 900 });
	GameEngineWindow::MainWindow.CursorOff();

	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<Lobby>("Lobby");

	GameEngineCore::CreateLevel<PlayLevel1>("PlayLevel1");
	GameEngineCore::CreateLevel<PlayLevel2>("PlayLevel2");
	//GameEngineCore::CreateLevel<PlayLevel3>("PlayLevel3");
	//GameEngineCore::CreateLevel<PlayLevel4>("PlayLevel4");

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