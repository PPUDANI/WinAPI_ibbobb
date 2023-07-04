#include "JumpingMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

#include "SoundLoadManager.h"

JumpingMonster::JumpingMonster()
{
}

JumpingMonster::~JumpingMonster()
{
}
void JumpingMonster::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");

	FilePath.MoveChild("Resources\\Texture\\Monster\\JumpingMonster\\");

	if (ResourcesManager::GetInst().FindSprite("JumpingMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("JumpingMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonsterCore.bmp"), 3, 3);
	}

	// Monster Animation
	{
		MonsterRenderer = CreateRenderer(RenderOrder::JumpingMonster);

		// Idle
		MonsterRenderer->CreateAnimation("Idle", "JumpingMonster.bmp", 0, 0, 10.0f, true);

		// Blink
		MonsterRenderer->CreateAnimation("Blink", "JumpingMonster.bmp", 0, 1, 0.1f, true);

		// Dead
		MonsterRenderer->CreateAnimation("Dead", "JumpingMonster.bmp", 2, 9, 0.03f, true);
	}
	
	// Monster�� Core ������ �� Core �ִϸ��̼�
	{
		CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);

		// Idle
		CoreRenderer->CreateAnimation("Idle", "JumpingMonsterCore.bmp", 0, 0, 10.0f, true);

		// Dead
		CoreRenderer->CreateAnimation("Dead", "JumpingMonsterCore.bmp", 1, 8, 0.01f, true);

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

	SetGravityPower(1000.0f);
	GravityDir = float4::DOWN;
	ReverseValue = false;
	DownCheck = float4::DOWN * BodyHalf;
	ChangeState(JumpingMonsterState::Fall);
}

void JumpingMonster::ReverseInit()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");

	FilePath.MoveChild("Resources\\Texture\\Monster\\JumpingMonster\\");

	if (ResourcesManager::GetInst().FindSprite("JumpingMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonster_Reverse.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("JumpingMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("JumpingMonsterCore.bmp"), 3, 3);
	}

	// Monster Animation
	{
		MonsterRenderer = CreateRenderer(RenderOrder::JumpingMonster);

		// Idle
		MonsterRenderer->CreateAnimation("Idle", "JumpingMonster_Reverse.bmp", 0, 0, 10.0f, true);

		// Blink
		MonsterRenderer->CreateAnimation("Blink", "JumpingMonster_Reverse.bmp", 0, 1, 0.1f, true);

		// Dead
		MonsterRenderer->CreateAnimation("Dead", "JumpingMonster_Reverse.bmp", 2, 9, 0.03f, true);
	}
	// Monster�� Core ������ �� Core �ִϸ��̼�
	{
		CoreRenderer = CreateRenderer(RenderOrder::MonsterCore);

		// Idle
		CoreRenderer->CreateAnimation("Idle", "JumpingMonsterCore.bmp", 0, 0, 10.0f, true);

		// Dead
		CoreRenderer->CreateAnimation("Dead", "JumpingMonsterCore.bmp", 1, 8, 0.01f, true);

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

	SetGravityPower(-1000.0f);
	GravityDir = float4::UP;
	ReverseValue = true;
	DownCheck = float4::UP * BodyHalf;
	ChangeState(JumpingMonsterState::Fall);
}

void JumpingMonster::Start()
{	
	if (GameEngineSound::FindSound("Death.mp3") == nullptr)
	{
		SoundLoadManager::LoadSound("Death", "Death.mp3");
	}
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
	// Core �浹 üũ
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

		EffectPlayer = GameEngineSound::SoundPlay("Death.mp3");
		EffectPlayer.SetVolume(1.0f);

		ChangeState(JumpingMonsterState::Dead);
		return;
	}

	// �ٴ� �浹 üũ
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


	// �ھ� ��ġ ����
	AddCoreVec = { 0.0f, (BodyHalf * 2) + BlockUnitHeight - (MovedPos * 2) };

	if (true == ReverseValue)
	{
		AddCoreVec.Y = -AddCoreVec.Y;
	}

	CoreRenderer->SetRenderPos(AddCoreVec);
	CoreCollision->SetCollisionPos(AddCoreVec);


	// Blink, Idle �ִϸ��̼� ���� ����
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
		Off();
	}
}


void JumpingMonster::SetAnimation(const std::string _State)
{
	MonsterRenderer->ChangeAnimation(_State);
}