#include "JumpingMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCollision.h>

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
	FilePath.MoveChild("Resources\\Texture\\Monster\\JumpingMonster\\");

	if (ResourcesManager::GetInst().FindSprite("JumpingMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster.bmp"), 3, 4);
	}

	if(ResourcesManager::GetInst().FindSprite("Reverse_JumpingMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Reverse_JumpingMonster.bmp"), 3, 4);
	}

	Renderer = CreateRenderer(RenderOrder::JumpingMonster);
	float Frame;

	// Idle
	Frame = 10.0f;
	Renderer->CreateAnimation("Idle", "JumpingMonster.bmp", 0, 0, Frame, true);
	Renderer->CreateAnimation("Reverse_Idle", "JumpingMonster.bmp", 0, 0, Frame, true);

	// Blink
	Frame = 0.1f;
	Renderer->CreateAnimation("Blink", "JumpingMonster.bmp", 0, 1, Frame, true);
	Renderer->CreateAnimation("Reverse_Blink", "JumpingMonster.bmp", 0, 1, Frame, true);

	// Dead
	Frame = 0.02f;
	Renderer->CreateAnimation("Dead", "JumpingMonster.bmp", 1, 11, Frame, true);
	Renderer->CreateAnimation("Reverse_Dead", "JumpingMonster.bmp", 1, 11, Frame, true);

	SetAnimation("Idle");

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60, 60 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	SetGravityPower(1100.0f);

	SetJumpForce(500.0f);
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

	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
	}

	if (true == Renderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}

void JumpingMonster::SetAnimation(const std::string _State)
{
	std::string AnimationName;
	if (true == ReverseValue)
	{
		AnimationName = "Reverse_";
	}

	AnimationName += _State;
	Renderer->ChangeAnimation(AnimationName);
}