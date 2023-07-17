#include "JumpingMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

#include "ContentsEnum.h"
#include "SoundLoadManager.h"

JumpingMonster::JumpingMonster()
{
}

JumpingMonster::~JumpingMonster()
{
}
void JumpingMonster::Init(const float4& _InitPos)
{

	// Monster Animation
	{
		MonsterRenderer = CreateRenderer(RenderOrder::JumpingMonster);

		// Idle
		MonsterRenderer->CreateAnimation("Idle", "JumpingMonster.bmp", 0, 0, 10.0f, true);

		// Blink
		MonsterRenderer->CreateAnimation("Blink", "JumpingMonster.bmp", 0, 1, 0.1f, false);

		// Dead
		MonsterRenderer->CreateAnimation("Dead", "JumpingMonster.bmp", 2, 9, 0.04f, false);

		// Live
		MonsterRenderer->CreateAnimation("Live", "JumpingMonster.bmp", 9, 2, 0.04f, false);

	}
	
	// Monster의 Core 렌더링 및 Core 애니메이션
	{
		CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);

		// Idle
		CoreRenderer->CreateAnimation("Idle", "JumpingMonsterCore.bmp", 0, 0, 10.0f, true);

		// Dead
		CoreRenderer->CreateAnimation("Dead", "JumpingMonsterCore.bmp", 1, 8, 0.04f, false);

		// Live
		CoreRenderer->CreateAnimation("Live", "JumpingMonsterCore.bmp", 8, 1, 0.04f, false);

		CoreRenderer->ChangeAnimation("Idle");
	}

	// MosterBody Collision
	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60, 60 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	// MonsterCore Collision
	CoreCollision = CreateCollision(CollisionOrder::MonsterCore);
	CoreCollision->SetCollisionScale({ 57, 57 });
	CoreCollision->SetCollisionType(CollisionType::CirCle);
	
	// Default Setting
	ChangeState(JumpingMonsterState::Fall);
	SetGravityPower(1000.0f);
	SetPos(_InitPos);

	GravityDir = float4::DOWN;
	ReverseValue = false;
	DownCheck = float4::DOWN * BodyHalf;
	StartVector = _InitPos;
}

void JumpingMonster::ReverseInit(const float4& _InitPos)
{
	// Monster Animation
	{
		MonsterRenderer = CreateRenderer(RenderOrder::JumpingMonster);

		// Idle
		MonsterRenderer->CreateAnimation("Idle", "JumpingMonster_Reverse.bmp", 0, 0, 10.0f, true);

		// Blink
		MonsterRenderer->CreateAnimation("Blink", "JumpingMonster_Reverse.bmp", 0, 1, 0.1f, false);

		// Dead
		MonsterRenderer->CreateAnimation("Dead", "JumpingMonster_Reverse.bmp", 2, 9, 0.04f, false);

		// Live
		MonsterRenderer->CreateAnimation("Live", "JumpingMonster_Reverse.bmp", 9, 2, 0.04f, false);
	}
	// Monster의 Core 렌더링 및 Core 애니메이션
	{
		CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);

		// Idle
		CoreRenderer->CreateAnimation("Idle", "JumpingMonsterCore.bmp", 0, 0, 10.0f, true);

		// Dead
		CoreRenderer->CreateAnimation("Dead", "JumpingMonsterCore.bmp", 1, 8, 0.04f, false);

		// Live
		CoreRenderer->CreateAnimation("Live", "JumpingMonsterCore.bmp", 8, 1, 0.04f, false);

		CoreRenderer->ChangeAnimation("Idle");
	}

	// MosterBody Collision
	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60, 60 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	// Monster Core Collision
	CoreCollision = CreateCollision(CollisionOrder::MonsterCore);
	CoreCollision->SetCollisionScale({ 57, 57 });
	CoreCollision->SetCollisionType(CollisionType::CirCle);

	// Default Setting
	ChangeState(JumpingMonsterState::Fall);
	SetGravityPower(-1000.0f);
	SetPos(_InitPos);

	GravityDir = float4::UP;
	ReverseValue = true;
	DownCheck = float4::UP * BodyHalf;
	StartVector = _InitPos;

	
}

void JumpingMonster::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");

	FilePath.MoveChild("Resources\\Texture\\Monster\\JumpingMonster\\");

	// JumpingMonster Texture Load
	if (ResourcesManager::GetInst().FindSprite("JumpingMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster.bmp"), 5, 2);
	}

	// JumpingMonster Reverse Texture Load
	if (ResourcesManager::GetInst().FindSprite("JumpingMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster_Reverse.bmp"), 5, 2);
	}

	// JumpingMonster Core Texture Load
	if (ResourcesManager::GetInst().FindSprite("JumpingMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonsterCore.bmp"), 3, 3);
	}

	SoundLoadManager::LoadSound("Monster", "MonsterDeath.mp3");
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
	case JumpingMonsterState::Live:
		LiveUpdate(_DeltaTime);
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
		BodyCollision->Off();
		CoreCollision->Off();
		EffectPlayer = GameEngineSound::SoundPlay("MonsterDeath.mp3");
		EffectPlayer.SetVolume(1.0f);

		ChangeState(JumpingMonsterState::Dead);
		return;
	}

	// 바닥 충돌 체크
	unsigned int DownColor = GetGroundColor(RGB(255, 0, 0), DownCheck);
	if (RGB(255, 0, 0) != DownColor)
	{
		if (true == ReverseValue)
		{
			MovedPos = PrevPos - GetPos().Y;
		}
		else
		{
			MovedPos = GetPos().Y - PrevPos;
		}
		
		Gravity(_DeltaTime);
	}
	else
	{
		while (RGB(255, 0, 0) != DownColor)
		{
			AddPos(-GravityDir);
		}
		AddPos(-GravityDir);
		PrevPos = GetPos().Y;
		GravityReset();
		SetGravityVector(-GravityDir * JumpForce);
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
		SetPos(StartVector);
		Off();
	}
}

void JumpingMonster::LiveUpdate(float _DeltaTime)
{
	if (MonsterRenderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
		SetCoreAnimation("Idle");
		BodyCollision->On();
		CoreCollision->On();
		ChangeState(JumpingMonsterState::Fall);
	}
}


void JumpingMonster::SetAnimation(const std::string _State)
{
	MonsterRenderer->ChangeAnimation(_State);
}

void JumpingMonster::SetCoreAnimation(const std::string _State)
{
	CoreRenderer->ChangeAnimation(_State);
}