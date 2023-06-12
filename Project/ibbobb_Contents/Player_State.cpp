#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

void Player::IdleUpdate(float _DeltaTime)
{
	// Couch
	if (true == GameEngineInput::IsDown(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		return;
	}

	// Run
	if (true == GameEngineInput::IsPress(MoveLeftKey) || true == GameEngineInput::IsPress(MoveRightKey))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump
	if (true == GameEngineInput::IsPress(JumpKey))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// Blink
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 500);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
	}

	if (true == MainRenderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}

	unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
	while (MiddleDownColor == RGB(255, 0, 0))
	{
		MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
		AddPos(float4::UP);
	}
}

void Player::CrouchUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		SetAnimation("Crouch");
		return;
	}
	else
	{
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
		return;
	}
}


void Player::RunUpdate(float _DeltaTime)
{
	// 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;
			
			// 좌측 벽 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
			if (LeftUpColor != RGB(255, 0, 0) && LeftMiddleColor != RGB(255, 0, 0) && LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 벽 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
			if (RightUpColor != RGB(255, 0, 0) && RightMiddleColor != RGB(255, 0, 0) && RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

		}
		else
		{
			ChangeState(PlayerState::Idle);
			SetAnimation("Idle");
			return;
		}
		AddPos(MovePos);
	}
	
	// Jump
	if (true == GameEngineInput::IsDown(JumpKey))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}
	
	// 바닥 체크
	{
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN);
		unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN);
		if (LeftDownColor != RGB(255, 0, 0) && MiddleDownColor != RGB(255, 0, 0) && RightDownColor != RGB(255, 0, 0))
		{
			SetAnimation("Fall");
			ChangeState(PlayerState::Fall);
			return;
		}
	}
	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// 바닥 체크
	{
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN - float4::LEFT);
		unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN - float4::RIGHT);
		if (LeftDownColor == RGB(255, 0, 0) || MiddleDownColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
		{
			GravityReset();
			ChangeState(PlayerState::Idle);
			SetAnimation("Idle");
		}
		else
		{
			Gravity(_DeltaTime);
		}
	}

	// 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 벽 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
			if (LeftUpColor != RGB(255, 0, 0) && LeftMiddleColor != RGB(255, 0, 0) && LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 벽 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
			if (RightUpColor != RGB(255, 0, 0) && RightMiddleColor != RGB(255, 0, 0) && RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

		}
		AddPos(MovePos);
	}
	
	if (true == MainRenderer->IsAnimationEnd())
	{
		SetAnimation("Fall");
	}

}


void Player::JumpUpdate(float _DeltaTime)
{
	// 천장 체크
	unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
	unsigned int MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
	unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
	if (LeftUpColor != RGB(255, 0, 0) && MiddleUpColor != RGB(255, 0, 0) && RightUpColor != RGB(255, 0, 0))
	{
		Gravity(_DeltaTime);
	}
	else
	{
		GravityReset();
		SetAnimation("Fall");
		ChangeState(PlayerState::Fall);
		return;
	}

	// 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 벽 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
			if (LeftUpColor != RGB(255, 0, 0) && LeftMiddleColor != RGB(255, 0, 0) && LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 벽 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
			if (RightUpColor != RGB(255, 0, 0) && RightMiddleColor != RGB(255, 0, 0) && RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

		}
		AddPos(MovePos);
	}

	// 일정 높이 이상 도달 체크
	if (-100.0f < GetGravityVector().Y)
	{
		SetAnimation("Tumbling");
		ChangeState(PlayerState::Fall);
		return;
	}

	SetAnimation("Jump");
}



void Player::DeadUpdate(float _DeltaTime)
{
	SetAnimation("Dead");
	if (true == MainRenderer->IsAnimationEnd())
	{
		Death();
	}
}