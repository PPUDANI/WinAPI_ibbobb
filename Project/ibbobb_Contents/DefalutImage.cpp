#include "DefaultImage.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

DefaultImage::DefaultImage()
{
}

DefaultImage::~DefaultImage()
{
}

void DefaultImage::Init(const std::string& _FileName, float _BlinkFrameSpeed/* = 0.5f*/)
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Image\\");

	if (ResourcesManager::GetInst().FindSprite(_FileName) == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath(_FileName), 1, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Image);

	// Blink Animation
	Renderer->CreateAnimation("Idle", _FileName, 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Blink", _FileName, 0, 1, _BlinkFrameSpeed, true);
}

void DefaultImage::Update(float _DeltaTime)
{
	if (true == LevitationValue)
	{
		LevitationUpdate(_DeltaTime);
	}

	if (true == BlinkValue)
	{
		Renderer->ChangeAnimation("Blink");
	}
	else
	{
		Renderer->ChangeAnimation("Idle");
	}
}

void DefaultImage::LevitationUpdate(float _DeltaTime)
{
	// 공중부양 연산
	{
		Radian += Speed * _DeltaTime;

		if (Radian >= GameEngineMath::PI2 + GameEngineMath::PI / 2.0f)
		{
			Radian = GameEngineMath::PI / 2.0f;
			SetPos(StartPos);
		}

		float4 AddPosY = { 0.0f, sinf(Radian) * MovingHeight };
		AddPos(AddPosY * _DeltaTime);
	}
}

void DefaultImage::LevelStart()
{
	StartPos = GetPos();
}



