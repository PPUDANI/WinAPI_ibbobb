#include "BlinkImage.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

BlinkImage::BlinkImage()
{
}

BlinkImage::~BlinkImage()
{
}

void BlinkImage::Init(const std::string& _FileName)
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Map\\");

	if (ResourcesManager::GetInst().FindSprite("Press_Text.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Press_Text.bmp"), 1, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Image);

	float Frame;

	// Blink Animation
	Frame = 0.5f;
	Renderer->CreateAnimation("Blink", "Press_Text.bmp", 0, 1, Frame, true);

	Renderer->ChangeAnimation("Blink");
}




