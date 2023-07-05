#include "LobbyLevelText.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"
LobbyLevelText::LobbyLevelText()
{
}

LobbyLevelText::~LobbyLevelText()
{
}

void LobbyLevelText::Init(const std::string _FileName, float _BlinkFrameSpeed/* = 0*/)
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Image\\");

	if (ResourcesManager::GetInst().FindSprite(_FileName) == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath(_FileName), 1, 3);
	}

	Renderer = CreateRenderer(RenderOrder::Image);

	Renderer->CreateAnimation("Idle", _FileName, 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Blink", _FileName, 0, 1, _BlinkFrameSpeed, true);
	Renderer->CreateAnimation("Deactivation", _FileName, 2, 2, 10.0f, true);
}

void LobbyLevelText::Update(float _DeltaTime)
{
	DefaultImage::Update(_DeltaTime);

	if (true == DeActivationValue)
	{
		Renderer->ChangeAnimation("Deactivation");
	}
}

