#include "GravityTransferPlatform.h"

#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "SoundLoadManager.h"
#include "ContentsEnum.h"
#include "ibb.h"
#include "obb.h"


GravityTransferPlatform::GravityTransferPlatform()
{
}

GravityTransferPlatform::~GravityTransferPlatform()
{
}

void GravityTransferPlatform::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");

	FilePath.MoveChild("Resources\\Texture\\GravityPlatform\\");

	if (nullptr == ResourcesManager::GetInst().FindSprite("GravityPlatform.bmp"))
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("GravityPlatform.bmp"), 1, 4);
	}

	PlatformRenderer = CreateRenderer(RenderOrder::GravityPlatform);

	PlatformRenderer->CreateAnimation("Idle", "GravityPlatform.bmp", 0, 0, 10.0f, true);

	PlatformRenderer->CreateAnimation("CantJumpSignal", "GravityPlatform.bmp", 3, 3, 0.1f, true);

	PlatformRenderer->CreateAnimation("TransferSignal", "GravityPlatform.bmp", 1, 0, 0.15f, false);
	

	PlatformCol = CreateCollision(CollisionOrder::GravityPlatform);
	PlatformCol->SetCollisionType(CollisionType::Rect);
	PlatformCol->SetCollisionScale({130.0f, 28.0f});
	
	State = PlatformState::Idle;
	SoundLoadManager::LoadSound("GravityPlatform", "CantJumpSignal.mp3");
}

void GravityTransferPlatform::Update(float _DeltaTime)
{
	switch (State)
	{
	case PlatformState::Idle:
		IdleUpdate(_DeltaTime);
		break;
	case PlatformState::ibbOnThisPlatform:
		ibbOnThisPlatformUpdate(_DeltaTime);
		break;
	case PlatformState::obbOnThisPlatform:
		obbOnThisPlatformUpdate(_DeltaTime);
		break;
	case PlatformState::TransferibbToobb:
		TransferibbToobbUpdate(_DeltaTime);
		break;
	case PlatformState::TransferobbToibb:
		TransferobbToibbUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void GravityTransferPlatform::IdleUpdate(float _DeltaTime)
{
	PlatformRenderer->ChangeAnimation("Idle");
	std::vector<GameEngineCollision*> _Col;

	if (true == PlatformCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		State = PlatformState::ibbOnThisPlatform;
	}
	else if (true == PlatformCol->Collision(CollisionOrder::obbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		State = PlatformState::obbOnThisPlatform;
	}
}

void GravityTransferPlatform::ibbOnThisPlatformUpdate(float _DeltaTime)
{
	std::vector<GameEngineCollision*> _Col;
	if (false == PlatformCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		State = PlatformState::Idle;
		TransferVectorInit = false;
		return;
	}
	
	if (true == PlatformCol->Collision(CollisionOrder::obbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect) &&
		PlayerState::Fall == obb::GetMainobb()->GetState() &&
		obb::GetMainobb()->GetReverseValue() != ibb::GetMainibb()->GetReverseValue())
	{
		State = PlatformState::TransferobbToibb;
	}

	if (true == PlatformCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect) && true == GameEngineInput::IsDown('W') ||
		true == PlatformCol->Collision(CollisionOrder::obbBody,
			_Col,
			CollisionType::Rect,
			CollisionType::Rect) && true == GameEngineInput::IsDown(VK_UP))
	{
		EffectPlayer = GameEngineSound::SoundPlay("CantJumpSignal.mp3");
		SetAnimation("CantJumpSignal");
	}

	if (true == PlatformRenderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}

void GravityTransferPlatform::obbOnThisPlatformUpdate(float _DeltaTime)
{
	std::vector<GameEngineCollision*> _Col;
	if (false == PlatformCol->Collision(CollisionOrder::obbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		TransferVectorInit = false;
		State = PlatformState::Idle;
		return;
	}

	if (true == PlatformCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect) &&
		PlayerState::Fall == ibb::GetMainibb()->GetState() &&
		obb::GetMainobb()->GetReverseValue() != ibb::GetMainibb()->GetReverseValue())
	{
		State = PlatformState::TransferibbToobb;
	}

	if (true == PlatformCol->Collision(CollisionOrder::ibbBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect) && true == GameEngineInput::IsDown('W') ||
		true == PlatformCol->Collision(CollisionOrder::obbBody,
			_Col,
			CollisionType::Rect,
			CollisionType::Rect) && true == GameEngineInput::IsDown(VK_UP))
	{
		EffectPlayer = GameEngineSound::SoundPlay("CantJumpSignal.mp3");
		SetAnimation("CantJumpSignal");
	}

	if (true == PlatformRenderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}

void GravityTransferPlatform::TransferibbToobbUpdate(float _DeltaTime)
{
	PlatformRenderer->ChangeAnimation("TransferSignal");
	if (false == TransferValue)
	{
		float4 ibbTransferVector = ibb::GetMainibb()->GetTransferVector();
		TransferValue = true;
		if (false == TransferVectorInit)
		{
			TransferVectorInit = true;
			TransferVector = ibbTransferVector;
		}
		else if (GetAbsoluteValue(ibbTransferVector.Y) > GetAbsoluteValue(TransferVector.Y) * 1.1f ||
			GetAbsoluteValue(ibbTransferVector.Y) < GetAbsoluteValue(TransferVector.Y) * 0.9f)
		{
			TransferVector = ibbTransferVector;
		}
		else
		{
			TransferVector = -TransferVector;
		}

		obb::GetMainobb()->SetGravityVector(TransferVector);
		obb::GetMainobb()->ChangeState(PlayerState::Jump);

		EffectPlayer = GameEngineSound::SoundPlay("CantJumpSignal.mp3");
	}

	if (true == PlatformRenderer->IsAnimationEnd())
	{
		TransferValue = false;
		State = PlatformState::Idle;
	}
}

void GravityTransferPlatform::TransferobbToibbUpdate(float _DeltaTime)
{
	PlatformRenderer->ChangeAnimation("TransferSignal");
	if (false == TransferValue)
	{
		float4 obbTransferVector = obb::GetMainobb()->GetTransferVector();
		TransferValue = true;

		if (false == TransferVectorInit)
		{
			TransferVectorInit = true;
			TransferVector = obbTransferVector;
		}
		else if (GetAbsoluteValue(obbTransferVector.Y) > GetAbsoluteValue(TransferVector.Y) * 1.1f ||
			GetAbsoluteValue(obbTransferVector.Y) < GetAbsoluteValue(TransferVector.Y) * 0.9f)
		{
			TransferVector = obbTransferVector;
		}
		else
		{
			TransferVector = -TransferVector;
		}

		ibb::GetMainibb()->SetGravityVector(TransferVector);
		ibb::GetMainibb()->ChangeState(PlayerState::Jump);

		EffectPlayer = GameEngineSound::SoundPlay("CantJumpSignal.mp3");
	}

	if (true == PlatformRenderer->IsAnimationEnd())
	{
		TransferValue = false;
		State = PlatformState::Idle;
	}
}

float GravityTransferPlatform::GetAbsoluteValue(float _Value)
{
	if(0.0f > _Value)
	{
		return -_Value;
	}
	else
	{
		return _Value;
	}
}

void GravityTransferPlatform::SetAnimation(const std::string _Name)
{
	PlatformRenderer->ChangeAnimation(_Name);
}