#include "LobbyMedal.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>

#include "ContentsEnum.h"
#include "Medal.h"


LobbyMedal::LobbyMedal()
{
}

LobbyMedal::~LobbyMedal()
{
}

void LobbyMedal::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Medal\\");

	if (ResourcesManager::GetInst().FindSprite("LobbyMedal.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("LobbyMedal.bmp"), 7, 1);
	}

	Renderer = CreateRenderer(RenderOrder::Medal);

	// Unacquired
	Renderer->CreateAnimation("Unacquired", "LobbyMedal.bmp", 0, 0, 10.0f, true);

	// Acquired
	Renderer->CreateAnimation("Acquired", "LobbyMedal.bmp", 1, 6, 0.1f, true);

	SetAnimation("Unacquired");
}

void LobbyMedal::LevelStart()
{
	
}

void LobbyMedal::SetAnimation(const std::string _Name)
{
	Renderer->ChangeAnimation(_Name);
}