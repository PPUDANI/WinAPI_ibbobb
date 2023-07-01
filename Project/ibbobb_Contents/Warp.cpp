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
	// 공중부양 연산
	{
		Radian += Speed;

		if (Radian == 360.0f)
		{
			Radian = 0.0f;
		}

		float4 AddPosY;
		if (WarpDir::Horizontal == Dir)
		{
			AddPosY = { 0.0f, sinf(Radian) * MovingHeight };
		}
		else if(WarpDir::Vertical == Dir)
		{
			AddPosY = { sinf(Radian) * MovingHeight, 0.0f };
		}
		
		for (int i = 0; i < StarNum; i++)
		{
			if (i % 2 == 0)
			{
				StarRenderer[i]->AddRenderPos(AddPosY * _DeltaTime);
			}
			else
			{
				StarRenderer[i]->AddRenderPos(-AddPosY * _DeltaTime );
			}
		}
	}
}