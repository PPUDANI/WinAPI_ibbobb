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
	FilePath.MoveChild("Resources\\Texture\\Monster\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster.bmp"), 3, 4);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RoadMonster.bmp"), 3, 4);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Turn_RoadMonster.bmp"), 5, 1);
	Renderer = CreateRenderer(RenderOrder::RoadMonster);

	// Idle
	Renderer->CreateAnimation("Left_Idle", "Left_RoadMonster.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Right_Idle", "Right_RoadMonster.bmp", 0, 0, 10.0f, true);

	// Blink
	Renderer->CreateAnimation("Left_Blink", "Left_RoadMonster.bmp", 0, 1, 0.08f, true);
	Renderer->CreateAnimation("Right_Blink", "Right_RoadMonster.bmp", 0, 1, 0.08f, true);

	// Dead
	Renderer->CreateAnimation("Left_Dead", "Left_RoadMonster.bmp", 1, 11, 0.02f, true);
	Renderer->CreateAnimation("Right_Dead", "Right_RoadMonster.bmp", 1, 11, 0.02f, true);

	// Turn
	Renderer->CreateAnimation("Left_Turn", "Turn_RoadMonster.bmp", 0, 4, 0.07f, true);
	Renderer->CreateAnimation("Right_Turn", "Turn_RoadMonster.bmp", 4, 0, 0.07f, true);



	Renderer->ChangeAnimation("Left_Idle");

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 58, 58 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	Speed = 0.1f;
}

void RoadMonster::Update(float _DeltaTime)
{
	static float CurDistance = 0.0f;
	float4 MovePos = float4::ZERO;

	if (CurDir == RoadMonsterDir::Left)
	{
		MovePos = float4::LEFT * Speed;
		
	}
	else
	{
		MovePos = float4::RIGHT * Speed;
	}

	if (CurDistance >= MovementDistance)
	{
		SetAnimation("Turn");
		if (Renderer->IsAnimationEnd())
		{
			CurDistance = 0.0f;
			if (CurDir == RoadMonsterDir::Right)
			{
				CurDir = RoadMonsterDir::Left;
			}
			else
			{
				CurDir = RoadMonsterDir::Right;
			}
			SetAnimation("Idle");
		}
	}
	else
	{
		CurDistance += Speed;
		AddPos(MovePos);
	}
	
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 500);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
	}
	if (true == Renderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}

void RoadMonster::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;

	switch (CurDir)
	{
	case RoadMonsterDir::Left:
		AnimationName = "Left_";
		break;
	case RoadMonsterDir::Right:
		AnimationName = "Right_";
		break;
	case RoadMonsterDir::ReverseLeft:
		AnimationName = "ReverseLeft_";
		break;
	case RoadMonsterDir::ReverseRight:
		AnimationName = "ReverseRight_";
		break;
	default:
		break;
	}

	AnimationName += _State;
	Renderer->ChangeAnimation(AnimationName, _StartFrame);
}