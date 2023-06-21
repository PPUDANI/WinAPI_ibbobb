#include "RoadMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "ContentsEnum.h"
RoadMonster::RoadMonster()
{

}

RoadMonster::~RoadMonster()
{

}

void RoadMonster::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Monster\\RoadMonster\\");

	if (ResourcesManager::GetInst().FindSprite("Left_RoadMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster.bmp"), 3, 4);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_RoadMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RoadMonster.bmp"), 3, 4);
	}

	if (ResourcesManager::GetInst().FindSprite("Turn_RoadMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Turn_RoadMonster.bmp"), 5, 1);
	}

	if (ResourcesManager::GetInst().FindSprite("Left_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster_Reverse.bmp"), 3, 4);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RoadMonster_Reverse.bmp"), 3, 4);
	}

	if (ResourcesManager::GetInst().FindSprite("Turn_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Turn_RoadMonster_Reverse.bmp"), 5, 1);
	}
	
	Renderer = CreateRenderer(RenderOrder::RoadMonster);

	// Idle
	Renderer->CreateAnimation("Left_Idle", "Left_RoadMonster.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Right_Idle", "Right_RoadMonster.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("ReverseLeft_Idle", "Left_RoadMonster_Reverse.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("ReverseRight_Idle", "Right_RoadMonster_Reverse.bmp", 0, 0, 10.0f, true);

	// Blink
	Renderer->CreateAnimation("Left_Blink", "Left_RoadMonster.bmp", 0, 1, 0.08f, true);
	Renderer->CreateAnimation("Right_Blink", "Right_RoadMonster.bmp", 0, 1, 0.08f, true);
	Renderer->CreateAnimation("ReverseLeft_Blink", "Left_RoadMonster_Reverse.bmp", 0, 1, 0.08f, true);
	Renderer->CreateAnimation("ReverseRight_Blink", "Right_RoadMonster_Reverse.bmp", 0, 1, 0.08f, true);

	// Dead
	Renderer->CreateAnimation("Left_Dead", "Left_RoadMonster.bmp", 1, 11, 0.02f, true);
	Renderer->CreateAnimation("Right_Dead", "Right_RoadMonster.bmp", 1, 11, 0.02f, true);
	Renderer->CreateAnimation("ReverseLeft_Dead", "Left_RoadMonster_Reverse.bmp", 1, 11, 0.02f, true);
	Renderer->CreateAnimation("ReverseRight_Dead", "Right_RoadMonster_Reverse.bmp", 1, 11, 0.02f, true);

	// Turn
	Renderer->CreateAnimation("Left_Turn", "Turn_RoadMonster.bmp", 0, 4, 0.07f, true);
	Renderer->CreateAnimation("Right_Turn", "Turn_RoadMonster.bmp", 4, 0, 0.07f, true);
	Renderer->CreateAnimation("ReverseLeft_Turn", "Turn_RoadMonster_Reverse.bmp", 0, 4, 0.07f, true);
	Renderer->CreateAnimation("ReverseRight_Turn", "Turn_RoadMonster_Reverse.bmp", 4, 0, 0.07f, true);
	Renderer->ChangeAnimation("Left_Idle");
	

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 40.0f, 40.0f }); // 58
	BodyCollision->SetCollisionType(CollisionType::CirCle);
}

void RoadMonster::Update(float _DeltaTime)
{
	float4 MovePos = float4::ZERO;

	if (CurDir == RoadMonsterDir::Left)
	{
		MovePos = float4::LEFT * Speed;
	}
	else if (CurDir == RoadMonsterDir::Right)
	{
		MovePos = float4::RIGHT * Speed;
	}

	if (CurDistance > MovementDistance)
	{
		SetAnimation("Turn");
		if (Renderer->IsAnimationEnd())
		{
			CurDistance = 0.0f;

			if (CurDir == RoadMonsterDir::Right)
			{
				CurDir = RoadMonsterDir::Left;
			}
			else if (CurDir == RoadMonsterDir::Left)
			{
				CurDir = RoadMonsterDir::Right;
			}

			SetAnimation("Idle");
		}
		return;
	}
	else
	{
		CurDistance += Speed;
		AddPos(MovePos);
	}

	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
		return;
	}

	if (true == Renderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}

void RoadMonster::SetAnimation(const std::string _State, int _StartFrame/* = 0*/)
{
	std::string AnimationName;
	if (true == ReverseValue)
	{
		AnimationName = "Reverse";
	}

	switch (CurDir)
	{
	case RoadMonsterDir::Left:
		AnimationName += "Left_";
		break;
	case RoadMonsterDir::Right:
		AnimationName += "Right_";
		break;
	default:
		break;
	}

	AnimationName += _State;
	Renderer->ChangeAnimation(AnimationName, _StartFrame);
}