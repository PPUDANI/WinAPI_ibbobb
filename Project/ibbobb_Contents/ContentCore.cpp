#include "ContentCore.h"

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
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");

	GameEngineCore::ChangeLevel("PlayLevel");
}

void ContentCore::Update(float _Delta)
{

}

void ContentCore::Render()
{

}

void ContentCore::Release()
{

}