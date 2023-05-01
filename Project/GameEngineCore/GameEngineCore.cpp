#include "GameEngineCore.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>

std::string GameEngineCore::WindowTitle = "";


GameEngineCore::GameEngineCore()
{

}

GameEngineCore::~GameEngineCore()
{

}

void GameEngineCore::CoreStart(HINSTANCE _Inst)
{
	GameEngineWindow::MainWindow.Open(WindowTitle, _Inst);
}

void GameEngineCore::CoreUpdate()
{

}

void GameEngineCore::CoreEnd()
{

}

void GameEngineCore::EngineStart(const std::string& _Title, HINSTANCE _Inst)
{
	GameEngineDebug::LeckCheck();

	WindowTitle = _Title;
	GameEngineWindow::MessageLoop(_Inst, CoreStart, CoreUpdate, CoreEnd);
}


