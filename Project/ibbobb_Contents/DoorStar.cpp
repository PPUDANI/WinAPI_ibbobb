#include "DoorStar.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

DoorStar::DoorStar()
{
}

DoorStar::~DoorStar()
{
}

void DoorStar::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\DoorStar\\");

	if (ResourcesManager::GetInst().FindSprite("DoorStar.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("DoorStar.bmp"), 5, 2);
	}
	if (ResourcesManager::GetInst().FindSprite("DeactivationStar.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("DeactivationStar.bmp"), 1, 1);
	}
	
	Renderer = CreateRenderer(RenderOrder::Image);

	// Turn
	Renderer->CreateAnimation("Turn", "DoorStar.bmp", 0, 9, 0.05f, true);

	// Deactivation
	Renderer->CreateAnimation("Deactivation", "DeactivationStar.bmp", 0, 0, 10.0f, true);


	
}

void DoorStar::Update(float _DeltaTime)
{
	if (true == ActivationValue)
	{
		Renderer->ChangeAnimation("Turn");
	}
	else
	{
		Renderer->ChangeAnimation("Deactivation");
	}
}
