#include "GameEngineActor.h"

#include "GameEngineRenderer.h"

GameEngineActor::GameEngineActor()
{

}

GameEngineActor::~GameEngineActor()
{

}

GameEngineRenderer* GameEngineActor::CreatreRenderer(const std::string& _ImageName)
{
	GameEngineRenderer* NewRenderer = new GameEngineRenderer();
	AllRenderer.push_back(NewRenderer);

	return nullptr;
}
