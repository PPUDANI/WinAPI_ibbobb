#include "ContentCore.h"

#include <GameEnginePlatform/GameEngineWindow.h>

#include "TitleLevel.h"
#include "Lobby.h"
#include "PlayLevel1.h"
#include "SubLevel.h"
ContentCore::ContentCore()
{

}

ContentCore::~ContentCore()
{

}


void ContentCore::Start()
{
	GameEngineWindow::MainWindow.SetPosAndScale({ 2000, 50 }, { 1500, 900 });
	GameEngineWindow::MainWindow.CursorOff();
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<PlayLevel1>("PlayLevel1");
	GameEngineCore::CreateLevel<Lobby>("Lobby");

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