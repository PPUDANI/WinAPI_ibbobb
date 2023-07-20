#include "Medal.h"

#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <cmath>

#include "ContentsEnum.h"
#include "SoundLoadManager.h"

std::vector<Medal*> Medal::MedalsByLevelIsAcquired;

Medal::Medal()
{
	
}

Medal::~Medal()
{
}

void Medal::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Medal\\");

	if (ResourcesManager::GetInst().FindSprite("LevelMedal.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("LevelMedal.bmp"), 7, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Medal);

	// 'Idle' Animation
	Renderer->CreateAnimation("Idle", "LevelMedal.bmp", 0, 8, 0.1f, true);

	// 'Get' Animation
	Renderer->CreateAnimation("Get", "LevelMedal.bmp", 9, 12, 0.02f, false);
	Renderer->CreateAnimation("Get1", "LevelMedal.bmp", 9, 12, 0.05f, false);
	Renderer->CreateAnimation("Get2", "LevelMedal.bmp", 9, 12, 0.05f, false);
	Renderer->CreateAnimation("Get3", "LevelMedal.bmp", 9, 12, 0.1f, false);
	Renderer->CreateAnimation("Get4", "LevelMedal.bmp", 9, 9, 0.3f, false);
	Renderer->CreateAnimation("Get5", "LevelMedal.bmp", 13, 13, 0.2f, false);

	// Medal Collision
	MedalCol = CreateCollision(CollisionOrder::Medal);
	MedalCol->SetCollisionType(CollisionType::CirCle);
	MedalCol->SetCollisionScale({ 56, 56 });

	// Default Setting
	CurState = MedalState::Idle;
	ReverseValue = false;
	MedalsByLevelIsAcquired.push_back(this);
}

void Medal::ReverseInit()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Medal\\");

	if (ResourcesManager::GetInst().FindSprite("LevelMedal_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("LevelMedal_Reverse.bmp"), 7, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Medal);

	// 'Idle' Animation
	Renderer->CreateAnimation("Idle", "LevelMedal_Reverse.bmp", 0, 8, 0.1f, true);

	// 'Get' Animation
	Renderer->CreateAnimation("Get", "LevelMedal_Reverse.bmp", 9, 12, 0.02f, false);
	Renderer->CreateAnimation("Get1", "LevelMedal_Reverse.bmp", 9, 12, 0.05f, false);
	Renderer->CreateAnimation("Get2", "LevelMedal_Reverse.bmp", 9, 12, 0.05f, false);
	Renderer->CreateAnimation("Get3", "LevelMedal_Reverse.bmp", 9, 12, 0.1f, false);
	Renderer->CreateAnimation("Get4", "LevelMedal_Reverse.bmp", 9, 9, 0.3f, false);
	Renderer->CreateAnimation("Get5", "LevelMedal_Reverse.bmp", 13, 13, 0.2f, false);

	// Medal Collision
	MedalCol = CreateCollision(CollisionOrder::Medal);
	MedalCol->SetCollisionType(CollisionType::CirCle);
	MedalCol->SetCollisionScale({ 56, 56 });

	// Default Setting
	CurState = MedalState::Idle;
	ReverseValue = true;
	MedalsByLevelIsAcquired.push_back(this);
}

void Medal::Start()
{
	SoundLoadManager::LoadSound("Medal", "GetMedal.mp3");
}	

void Medal::Update(float _DeltaTime)
{
	switch (CurState)
	{
	case MedalState::Idle:
		IdleUpdate(_DeltaTime);
		break;
	case MedalState::Get:
		GetUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Medal::LevelStart()
{
	StartPos = GetPos();
}

void Medal::IdleUpdate(float _DeltaTime)
{
	std::vector<GameEngineCollision*> _Col;
	if (true == MedalCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::CirCle,
		CollisionType::Rect) ||
		true == MedalCol->Collision(CollisionOrder::obbBody,
		_Col,
		CollisionType::CirCle,
		CollisionType::Rect))
	{
		AcquiredValue = true;
		CurState = MedalState::Get;

		EffectPlayer = GameEngineSound::SoundPlay("GetMedal.mp3");
		EffectPlayer.SetVolume(0.2f);
		SetAnimation("Get");
		return;
	}

	// 공중부양 연산
	Levitation(_DeltaTime);

	SetAnimation("Idle");
}

void Medal::GetUpdate(float _DeltaTime)
{
	if (Renderer->IsAnimationEnd())
	{
		Count++;
		switch (Count)
		{
		case 1:
			SetAnimation("Get1");
			break;
		case 2:
			SetAnimation("Get2");
			break;
		case 3:
			SetAnimation("Get3");
			break;
		case 4:
			SetAnimation("Get4");
			break;
		case 5:
			SetAnimation("Get5");
			break;
		default:
			Off();
			break;
		}
	}
}

void Medal::Levitation(float _DeltaTime)
{
	Radian += Speed * _DeltaTime;

	if (Radian >= GameEngineMath::PI2 + GameEngineMath::PI / 2.0f)
	{
		Radian = GameEngineMath::PI / 2.0f;
		SetPos(StartPos);
	}

	float4 AddPosY = { 0.0f, sinf(Radian) * MovingHeight };
	AddPos(AddPosY * _DeltaTime);
}

void Medal::SetAnimation(const std::string& _Name)
{
	Renderer->ChangeAnimation(_Name);
}