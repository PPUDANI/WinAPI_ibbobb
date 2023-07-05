#include "Titleobb.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include "ContentsEnum.h"

Titleobb::Titleobb()
{
}

Titleobb::~Titleobb()
{
}

void Titleobb::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\obb\\");

	if (ResourcesManager::GetInst().FindSprite("Left_obb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_obb.bmp"), 7, 6);
	}

	obbRenderer = CreateRenderer(RenderOrder::obb);
	float Frame;

	// Idle Animation
	Frame = 10.0f;
	obbRenderer->CreateAnimation("Idle", "Left_obb.bmp", 0, 0, Frame, true);

	// Blink Animation
	Frame = 0.07f;
	obbRenderer->CreateAnimation("Blink", "Left_obb.bmp", 0, 1, Frame, false);

	// Crouch Animation
	Frame = 0.1f;
	obbRenderer->CreateAnimation("Crouch", "Left_obb.bmp", 11, 11, Frame, true);

	obbRenderer->SetScaleRatio(15.0f);
	obbRenderer->ChangeAnimation("Idle");
}

void Titleobb::Update(float _DeltaTime)
{
	// Crouch 상태 체크
	if (true == GameEngineInput::IsPress(VK_DOWN))
	{
		obbRenderer->ChangeAnimation("Crouch");
		return;
	}

	// Blink, Idle 애니메이션 랜덤 교차
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);
	if (RandomNumber == 1)
	{
		obbRenderer->ChangeAnimation("Blink");
	}
	if (true == obbRenderer->IsAnimationEnd())
	{
		obbRenderer->ChangeAnimation("Idle");
	}
}

