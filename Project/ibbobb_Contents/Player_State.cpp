#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"
#include <vector>
void Player::IdleUpdate(float _DeltaTime)
{
	unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
	while (MiddleDownColor == RGB(255, 0, 0))
	{
		MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
		AddPos(float4::UP);
	}

	// Couch
	if (true == GameEngineInput::IsPress(CrouchKey))
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
}

void Player::CrouchUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::Left;
	}
	else if(true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::Right;
	}

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
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Idle);
		SetAnimation("Crouch");
		return;
	}
	// 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;
			
			// 좌측 충돌 체크
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

			// 우측 충돌 체크
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
	
	// 바닥 충돌 체크
	{
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN);
		unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN);
		if (LeftDownColor != RGB(255, 0, 0) && MiddleDownColor != RGB(255, 0, 0) && RightDownColor != RGB(255, 0, 0))
		{
			ChangeState(PlayerState::Fall);
			IsJump = false;
			return;
		}
	}
	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{

	{
		unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		if (MidColor == RGB(0, 255, 255))
		{
			SetGravityPower(-1200);
			if (CurDir == PlayerDir::Left)
			{
				CurDir = PlayerDir::ReverseLeft;
			}
			else if (CurDir == PlayerDir::Right)
			{
				CurDir = PlayerDir::ReverseRight;
			}
		}
		else
		{
			SetGravityPower(1200);
			if (CurDir == PlayerDir::ReverseLeft)
			{
				CurDir = PlayerDir::Left;
			}
			else if(CurDir == PlayerDir::ReverseRight)
			{
				CurDir = PlayerDir::Right;
			}
		}
	}

	// 바닥 충돌 체크
	{
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN - float4::LEFT);
		unsigned int MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN - float4::RIGHT);
		if (LeftDownColor == RGB(255, 0, 0) || MiddleDownColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
		{
			GravityReset();
			SetAnimation("Idle");
			ChangeState(PlayerState::Idle);
			return;
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

			// 좌측 충돌 체크
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

			// 우측 충돌 체크
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

	// 점프에서 Fall로 바뀐지 체크 (IsJump가 true면 점프에서 상태가 Fall로 바뀐것임.)
	if (IsJump == true)
	{
		int CurFrame = static_cast<int>(MainRenderer->GetCurFrame());
		SetAnimation("Tumbling", CurFrame);
		if (true == MainRenderer->IsAnimationEnd())
		{
			IsJump = false;
		}
	}
	else
	{
		SetAnimation("Fall");
	}
}

void Player::JumpUpdate(float _DeltaTime)
{
	// 상단 충돌 체크
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
		ChangeState(PlayerState::Fall);
		return;
	}	

	// 일정 높이 이상 도달 체크
	if (-100.0f < GetGravityVector().Y)
	{
		IsJump = true;
		SetAnimation("Tumbling");
		ChangeState(PlayerState::Fall);
		return;
	}

	// 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
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

			// 우측 충돌 체크
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