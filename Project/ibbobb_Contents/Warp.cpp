#include "Warp.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

Warp::Warp()
{
}

Warp::~Warp()
{
}

void Warp::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Warp\\");

	if (ResourcesManager::GetInst().FindSprite("WarpStar_Yellow.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("WarpStar_Yellow.bmp"), 4, 1);
	}

	for (int i = 0; i < StarNum; i++)
	{
		GameEngineRenderer* Renderer = CreateRenderer(RenderOrder::Warp);

		Renderer->CreateAnimation("Turn", "WarpStar_Yellow.bmp", 0, 3, 0.1f, true);
		if (WarpDir::Horizontal == Dir)
		{
			Renderer->SetRenderPos({ i * 13.0f, 0.0f });
		}
		else if (WarpDir::Vertical == Dir)
		{
			Renderer->SetRenderPos({ 0.0f, i * 13.0f });
		}
		
		Renderer->ChangeAnimation("Turn", i % 4);
		StarRenderer.push_back(Renderer);
	}
}

void Warp::Start()
{

}

void Warp::Update(float _DeltaTime)
{

}