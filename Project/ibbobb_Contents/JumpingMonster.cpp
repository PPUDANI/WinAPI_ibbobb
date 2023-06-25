#include "JumpingMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>


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
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("JumpingMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster_Reverse.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("JumpingMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonsterCore.bmp"), 3, 3);
	}

	MonsterRenderer = CreateRenderer(RenderOrder::JumpingMonster);
	float Frame;

	// Idle
	Frame = 10.0f;
	MonsterRenderer->CreateAnimation("Idle", "JumpingMonster.bmp", 0, 0, Frame, true);
	MonsterRenderer->CreateAnimation("Reverse_Idle", "JumpingMonster_Reverse.bmp", 0, 0, Frame, true);

	// Blink
	Frame = 0.1f;
	MonsterRenderer->CreateAnimation("Blink", "JumpingMonster.bmp", 0, 1, Frame, true);
	MonsterRenderer->CreateAnimation("Reverse_Blink", "JumpingMonster_Reverse.bmp", 0, 1, Frame, true);

	// Dead
	Frame = 0.03f;
	MonsterRenderer->CreateAnimation("Dead", "JumpingMonster.bmp", 2, 9, Frame, true);
	MonsterRenderer->CreateAnimation("Reverse_Dead", "JumpingMonster_Reverse.bmp", 2, 9, Frame, true);


	// Monster의 Core 렌더링 및 Core 애니메이션
	CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);

	Frame = 10.0f;
	CoreRenderer->CreateAnimation("Idle", "JumpingMonsterCore.bmp", 0, 0, Frame, true);
	Frame = 0.01f;
	CoreRenderer->CreateAnimation("Dead", "JumpingMonsterCore.bmp", 1, 8, Frame, true);
	CoreRenderer->ChangeAnimation("Idle");

	// MosterBody Collision
	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60, 60 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	// Monster Core Collision
	CoreCollision = CreateCollision(CollisionOrder::MonsterCore);
	CoreCollision->SetCollisionScale({ 57, 57 });
	CoreCollision->SetCollisionType(CollisionType::CirCle);

	SetAnimation("Idle");
	SetGravityPower(1100.0f);
	
	ChangeState(JumpingMonsterState::Fall);

	DownCheck = float4::DOWN * BodyHalf;
}

void JumpingMonster::Update(float _DeltaTime)
{
	switch (CurState)
	{
	case JumpingMonsterState::Fall:
		FallUpdate(_DeltaTime);
		break;
	case JumpingMonsterState::Dead:
		DeadUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void JumpingMonster::FallUpdate(float _DeltaTime)
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
		ChangeState(JumpingMonsterState::Dead);
	}


	// 바닥 충돌 체크
	unsigned int Color = GetGroundColor(RGB(255, 0, 0), DownCheck);
	if (RGB(255, 0, 0) != Color)
	{
		MovedPos = GetPos().Y - PrevPos;

		Gravity(_DeltaTime);
	}
	else
	{
		PrevPos = GetPos().Y;
		AddPos(float4::UP);
		SetGravityVector(float4::UP * JumpForce);
	}


	// 코어 위치 연산
	AddCoreVec = { 0.0f, (BodyHalf * 2) + BlockUnitHeight - (MovedPos * 2) };
	if (true == ReverseValue)
	{
		AddCoreVec.Y = -AddCoreVec.Y;
	}
	CoreRenderer->SetRenderPos(AddCoreVec);
	CoreCollision->SetCollisionPos(AddCoreVec);


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

void JumpingMonster::DeadUpdate(float _DeltaTime)
{
	if (MonsterRenderer->IsAnimationEnd())
	{
		Death();
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
	MonsterRenderer->ChangeAnimation(AnimationName);
}