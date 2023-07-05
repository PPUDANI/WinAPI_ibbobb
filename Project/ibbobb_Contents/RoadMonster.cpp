#include "RoadMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include <GameEnginePlatform/GameEngineInput.h>
#include "SoundLoadManager.h"
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
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_RoadMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RoadMonster.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("Turn_RoadMonster.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Turn_RoadMonster.bmp"), 5, 1);
	}

	if (ResourcesManager::GetInst().FindSprite("RoadMonsterCore_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("RoadMonsterCore_Reverse.bmp"), 3, 3);
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
	MonsterRenderer->CreateAnimation("Left_Blink", "Left_RoadMonster.bmp", 0, 1, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Blink", "Right_RoadMonster.bmp", 0, 1, Frame, false);

	// Dead
	Frame = 0.02f;
	MonsterRenderer->CreateAnimation("Left_Dead", "Left_RoadMonster.bmp", 2, 9, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Dead", "Right_RoadMonster.bmp", 2, 9, Frame, false);
	CoreRenderer->CreateAnimation("Dead", "RoadMonsterCore_Reverse.bmp", 1, 8, Frame, false);

	// Turn
	Frame = 0.05f;
	MonsterRenderer->CreateAnimation("Left_Turn", "Turn_RoadMonster.bmp", 0, 4, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Turn", "Turn_RoadMonster.bmp", 4, 0, Frame, false);

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

	ReverseValue = false;
}

void RoadMonster::ReverseInit()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Monster\\RoadMonster\\");

	if (ResourcesManager::GetInst().FindSprite("Left_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster_Reverse.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_RoadMonster_Reverse.bmp"), 5, 2);
	}

	if (ResourcesManager::GetInst().FindSprite("Turn_RoadMonster_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Turn_RoadMonster_Reverse.bmp"), 5, 1);
	}

	if (ResourcesManager::GetInst().FindSprite("RoadMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("RoadMonsterCore.bmp"), 3, 3);
	}
		if (ResourcesManager::GetInst().FindSprite("RoadMonsterCore.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("RoadMonsterCore.bmp"), 3, 3);
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
	MonsterRenderer->CreateAnimation("Left_Blink", "Left_RoadMonster_Reverse.bmp", 0, 1, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Blink", "Right_RoadMonster_Reverse.bmp", 0, 1, Frame, false);

	// Dead
	Frame = 0.02f;
	MonsterRenderer->CreateAnimation("Left_Dead", "Left_RoadMonster_Reverse.bmp", 2, 9, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Dead", "Right_RoadMonster_Reverse.bmp", 2, 9, Frame, false);
	CoreRenderer->CreateAnimation("Dead", "RoadMonsterCore.bmp", 1, 8, Frame, false);

	// Turn
	Frame = 0.05f;
	MonsterRenderer->CreateAnimation("Left_Turn", "Turn_RoadMonster_Reverse.bmp", 0, 4, Frame, false);
	MonsterRenderer->CreateAnimation("Right_Turn", "Turn_RoadMonster_Reverse.bmp", 4, 0, Frame, false);

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

	ReverseValue = true;
}

void RoadMonster::Start()
{
	SoundLoadManager::LoadSound("Monster", "MonsterDeath.mp3");
}

void RoadMonster::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		CheckPosOn = !CheckPosOn;
	}

	switch (CurState)
	{
	case RoadMonsterState::Move:
		MoveUpdate(_DeltaTime);
		break;
	case RoadMonsterState::Dead:
		DeadUpdate(_DeltaTime);
		break;
	case RoadMonsterState::Turn:
		TurnUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void RoadMonster::Render(float _DeltaTime)
{
	if(true == CheckPosOn)
	{
		CollisionData Data;
		HDC dc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
		Data.Scale = { 3, 3 };
		Data.Pos = ActorCameraPos() + LeftCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + RightCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		if (true == ReverseValue)
		{
			Data.Pos = ActorCameraPos() + ReverseLeftDownCheck;
			Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
			Data.Pos = ActorCameraPos() + ReverseRightDownCheck;
			Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		}
		else
		{
			Data.Pos = ActorCameraPos() + LeftDownCheck;
			Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
			Data.Pos = ActorCameraPos() + RightDownCheck;
			Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
			
		}
	}
}

void RoadMonster::MoveUpdate(float _DeltaTime)
{
	// Core 충돌 체크
	std::vector<GameEngineCollision*> _Col;
	if (CoreCollision->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect) ||
		CoreCollision->Collision(CollisionOrder::obbBody,
			_Col,
			CollisionType::Rect,
			CollisionType::Rect)
		)
	{
		SetAnimation("Dead");
		CoreRenderer->ChangeAnimation("Dead");
		EffectPlayer = GameEngineSound::SoundPlay("MonsterDeath.mp3");
		EffectPlayer.SetVolume(1.0f);

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

	if (false == MovePossibleCheck())
	{
		SetAnimation("Turn");
		ChangeState(RoadMonsterState::Turn);
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

void RoadMonster::TurnUpdate(float _DeltaTime)
{
	if (MonsterRenderer->IsAnimationEnd())
	{
		CurDistance = 0.0f;

		if (CurDir == RoadMonsterDir::Right)
		{
			CurDir = RoadMonsterDir::Left;
			AddPos(float4::LEFT);
		}
		else if (CurDir == RoadMonsterDir::Left)
		{
			CurDir = RoadMonsterDir::Right;
			AddPos(float4::RIGHT);
		}
		SetAnimation("Idle");
		ChangeState(RoadMonsterState::Move);
		return;
	}
}

void RoadMonster::DeadUpdate(float _DeltaTime)
{
	if (MonsterRenderer->IsAnimationEnd())
	{
		Off();
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

bool RoadMonster::MovePossibleCheck()
{
	unsigned int LeftColor = GetGroundColor(RGB(255, 0, 0), LeftCheck);
	unsigned int RightColor = GetGroundColor(RGB(255, 0, 0), RightCheck);
	unsigned int LeftDownColor;
	unsigned int RightDownColor;

	if (true == ReverseValue)
	{
		LeftDownColor = GetGroundColor(RGB(255, 0, 0), ReverseLeftDownCheck);
		RightDownColor = GetGroundColor(RGB(255, 0, 0), ReverseRightDownCheck);
	}
	else
	{
		LeftDownColor = GetGroundColor(RGB(255, 0, 0), LeftDownCheck);
		RightDownColor = GetGroundColor(RGB(255, 0, 0), RightDownCheck);
	}

	if (RoadMonsterDir::Left == CurDir)
	{
		if (RGB(255, 0, 0) == LeftColor ||
			RGB(255, 0, 0) != LeftDownColor)
		{
			return false;
		}
	}
	else if (RoadMonsterDir::Right == CurDir)
	{
		if (RGB(255, 0, 0) == RightColor ||
			RGB(255, 0, 0) != RightDownColor)
		{
			return false;
		}
	}

	std::vector<GameEngineCollision*> _Col;
	if (BodyCollision->Collision(CollisionOrder::MonsterBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		for (int i = 0; i < _Col.size(); i++)
		{
			if (_Col[i] == BodyCollision)
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}