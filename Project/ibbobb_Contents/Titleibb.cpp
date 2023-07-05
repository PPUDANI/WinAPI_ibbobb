#include "Titleibb.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "ContentsEnum.h"

Titleibb::Titleibb()
{
}

Titleibb::~Titleibb()
{
}

void Titleibb::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\ibb\\");

	if (ResourcesManager::GetInst().FindSprite("Right_ibb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_ibb.bmp"), 7, 6);
	}

	ibbRenderer = CreateRenderer(RenderOrder::ibb);

	float Frame;
	// Idle Animation
	Frame = 10.0f;
	ibbRenderer->CreateAnimation("Idle", "Right_ibb.bmp", 0, 0, Frame, true);

	// Blink Animation
	Frame = 0.07f;
	ibbRenderer->CreateAnimation("Blink", "Right_ibb.bmp", 0, 1, Frame, false);

	// Crouch Animation
	Frame = 0.1f;
	ibbRenderer->CreateAnimation("Crouch", "Right_ibb.bmp", 11, 11, Frame, true);

	ibbRenderer->SetScaleRatio(15.0f);
	ibbRenderer->ChangeAnimation("Idle");
}

void Titleibb::Update(float _DeltaTime)
{
	// Crouch 상태 체크
	if (true == GameEngineInput::IsPress('S'))
	{
		ibbRenderer->ChangeAnimation("Crouch");
		return;
	}

	// Blink, Idle 애니메이션 랜덤 교차
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);
	if (RandomNumber == 1)
	{
		ibbRenderer->ChangeAnimation("Blink");
	}
	if (true == ibbRenderer->IsAnimationEnd())
	{
		ibbRenderer->ChangeAnimation("Idle");
	}
}