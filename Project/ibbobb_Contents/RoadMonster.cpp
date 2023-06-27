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

void RoadMonster::Init()
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

	if (ResourcesManager::GetInst().FindSprite("RoadMonsterCore_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("RoadMonsterCore_Reverse.bmp"), 5, 2);
	}

	MonsterRenderer = CreateRenderer(RenderOrder::RoadMonster);
	CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);
	float Frame;

	// Idle
	Frame = 10.0f;
	MonsterRenderer->CreateAnimation("Left_Idle", "Left_RoadMonster.bmp", 0, 0, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Idle", "Right_RoadMonster.bmp", 0, 0, Frame, true);
	CoreRenderer->CreateAnimation("Idle", "RoadMonsterCore_Reverse.bmp", 0, 0, Frame, true);

	// Blink
	Frame = 0.1f;
	MonsterRenderer->CreateAnimation("Left_Blink", "Left_RoadMonster.bmp", 0, 1, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Blink", "Right_RoadMonster.bmp", 0, 1, Frame, true);

	// Dead
	Frame = 0.02f;
	MonsterRenderer->CreateAnimation("Left_Dead", "Left_RoadMonster.bmp", 1, 11, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Dead", "Right_RoadMonster.bmp", 1, 11, Frame, true);
	CoreRenderer->CreateAnimation("Dead", "RoadMonsterCore_Reverse.bmp", 1, 9, Frame, true);

	// Turn
	Frame = 0.07f;
	MonsterRenderer->CreateAnimation("Left_Turn", "Turn_RoadMonster.bmp", 0, 4, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Turn", "Turn_RoadMonster.bmp", 4, 0, Frame, true);

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60.0f, 30.0f });
	BodyCollision->SetCollisionType(CollisionType::Rect);
	BodyCollision->SetCollisionPos({ 0.0f, -14.0f });

	
	CoreCollision = CreateCollision(CollisionOrder::MonsterCore);
	CoreCollision->SetCollisionScale({ 60.0f, 30.0f });
	CoreCollision->SetCollisionType(CollisionType::Rect);
	CoreCollision->SetCollisionPos({ 0.0f, 42.0f });

	CoreRenderer->ChangeAnimation("Idle");
	CoreRenderer->SetRenderPos({ 0.0f, 27.0f });

	CurState = RoadMonsterState::Move;
}

void RoadMonster::ReverseInit()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Monster\\RoadMonster\\");

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

	if (ResourcesManager::GetInst().FindSprite("RoadMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("RoadMonsterCore.bmp"), 5, 2);
	}


	MonsterRenderer = CreateRenderer(RenderOrder::RoadMonster);
	CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);
	float Frame;

	// Idle
	Frame = 10.0f;
	MonsterRenderer->CreateAnimation("Left_Idle", "Left_RoadMonster_Reverse.bmp", 0, 0, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Idle", "Right_RoadMonster_Reverse.bmp", 0, 0, Frame, true);
	CoreRenderer->CreateAnimation("Idle", "RoadMonsterCore.bmp", 0, 0, Frame, true);

	// Blink
	Frame = 0.1f;
	MonsterRenderer->CreateAnimation("Left_Blink", "Left_RoadMonster_Reverse.bmp", 0, 1, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Blink", "Right_RoadMonster_Reverse.bmp", 0, 1, Frame, true);

	// Dead
	Frame = 0.02f;
	MonsterRenderer->CreateAnimation("Left_Dead", "Left_RoadMonster_Reverse.bmp", 1, 11, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Dead", "Right_RoadMonster_Reverse.bmp", 1, 11, Frame, true);
	CoreRenderer->CreateAnimation("Dead", "RoadMonsterCore.bmp", 1, 9, Frame, true);

	// Turn
	Frame = 0.07f;
	MonsterRenderer->CreateAnimation("Left_Turn", "Turn_RoadMonster_Reverse.bmp", 0, 4, Frame, true);
	MonsterRenderer->CreateAnimation("Right_Turn", "Turn_RoadMonster_Reverse.bmp", 4, 0, Frame, true);

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60.0f, 30.0f });
	BodyCollision->SetCollisionType(CollisionType::Rect);
	BodyCollision->SetCollisionPos({ 0.0f, 14.0f });

	CoreCollision = CreateCollision(CollisionOrder::MonsterCore);
	CoreCollision->SetCollisionScale({ 60.0f, 30.0f });
	CoreCollision->SetCollisionType(CollisionType::Rect);
	CoreCollision->SetCollisionPos({ 0.0f, -42.0f });

	CoreRenderer->ChangeAnimation("Idle");
	CoreRenderer->SetRenderPos({ 0.0f, -27.0f });

	CurState = RoadMonsterState::Move;
}

void RoadMonster::Start()
{
}

void RoadMonster::Update(float _DeltaTime)
{
	switch (CurState)
	{
	case RoadMonsterState::Move:
		MoveUpdate(_DeltaTime);
		break;
	case RoadMonsterState::Dead:
		DeadUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}
void RoadMonster::MoveUpdate(float _DeltaTime)
{
	// Core 충돌 체크
	std::vector<GameEngineCollision*> _Col;
	if (CoreCollision->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::CirCle,
		CollisionType::Rect) ||
		CoreCollision->Collision(CollisionOrder::obbBody,
			_Col,
			CollisionType::CirCle,
			CollisionType::Rect)
		)
	{
		SetAnimation("Dead");
		CoreRenderer->ChangeAnimation("Dead");
		CurState = RoadMonsterState::Dead;
		return;
	}


	if (MonsterRenderer == nullptr)
	{
		MsgBoxAssert("RoadMonster에 Init()을 호출하지 않았습니다.");
	}

	float4 MovePos = float4::ZERO;

	if (CurDir == RoadMonsterDir::Left)
	{
		MovePos = float4::LEFT * Speed * _DeltaTime;
	}
	else if (CurDir == RoadMonsterDir::Right)
	{
		MovePos = float4::RIGHT * Speed * _DeltaTime;
	}

	if (CurDistance > MovementDistance)
	{
		SetAnimation("Turn");
		if (MonsterRenderer->IsAnimationEnd())
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
		CurDistance += Speed * _DeltaTime;
		AddPos(MovePos);
	}

	// Blink, Idle 애니메이션 랜덤 교차
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
		IsAnimBlink = true;
	}
	if (true == IsAnimBlink)
	{
		if (true == MonsterRenderer->IsAnimationEnd())
		{
			IsAnimBlink = true;
			SetAnimation("Idle");
		}
	}
	else
	{
		SetAnimation("Idle");
	}
}

void RoadMonster::DeadUpdate(float _DeltaTime)
{
	if (MonsterRenderer->IsAnimationEnd())
	{
		Death();
	}
}

void RoadMonster::SetAnimation(const std::string _State, int _StartFrame/* = 0*/)
{
	std::string AnimationName;


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
	MonsterRenderer->ChangeAnimation(AnimationName, _StartFrame);
}

