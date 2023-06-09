#include "JumpingMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>

JumpingMonster::JumpingMonster()
{
}

JumpingMonster::~JumpingMonster()
{
}

void JumpingMonster::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Monster\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster.bmp"), 3, 4);
	Renderer = CreateRenderer(RenderOrder::JumpingMonster);

	Renderer->CreateAnimation("Idle", "JumpingMonster.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Blink", "JumpingMonster.bmp", 0, 1, 0.05f, true);
	Renderer->CreateAnimation("Dead", "JumpingMonster.bmp", 1, 11, 0.02f, true);
	Renderer->ChangeAnimation("Idle");

	SetGravityPower(2.5f);

	SetJumpForce(1.0f);
	
	SetPos({ 400.0f, 600.0f });
}

void JumpingMonster::Update(float _DeltaTime)
{
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), float4::DOWN* 32.0f);

	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		AddPos(float4::UP);
		SetGravityVector(float4::UP * JumpForce);
	}

	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 400);
	if (RandomNumber == 1)
	{
		Renderer->ChangeAnimation("Blink");
	}
	if (true == Renderer->IsAnimationEnd())
	{
		Renderer->ChangeAnimation("Idle");
	}
}

