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
	// 바닥에 닿을 시 워프 시 추가된 중력 제거
	{
		if (true == ReverseValue)
		{
			SetGravityPower(-DefaultGravityPower);
		}
		else
		{
			SetGravityPower(DefaultGravityPower);
		}
	}

	// Run 상태 체크

	if (true == GameEngineInput::IsPress(MoveLeftKey) || true == GameEngineInput::IsPress(MoveRightKey))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump 상태 체크
	if (true == GameEngineInput::IsPress(JumpKey))
	{
		if (true == ReverseValue)
		{
			SetGravityVector(CurGravity * JumpForce);
		}
		else
		{
			SetGravityVector(-CurGravity * JumpForce);
		}
		ChangeState(PlayerState::Jump);
		return;
	}
	
	// Crouch 상태 체크
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		return;
	}

	// Blink, Idle 애니메이션 랜덤 교차
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
	// 방향 체크
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::Right;
	}

	// 키를 떼면 Idle 상태로 변환
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		SetAnimation("Crouch");
	}
	else
	{
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
	}
}


void Player::RunUpdate(float _DeltaTime)
{
	// Run의 Crouch 상태 체크
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		SetAnimation("Crouch");
		return;
	}

	// Run의 Jump 상태 체크
	if (true == GameEngineInput::IsDown(JumpKey))
	{
		if (true == ReverseValue)
		{
			SetGravityVector(CurGravity * JumpForce);
		}
		else
		{
			SetGravityVector(-CurGravity * JumpForce);
		}

		ChangeState(PlayerState::Jump);
		return;
	}

	// Run의 중력 반전 체크
	{
		unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		if (MidColor == RGB(0, 255, 255))
		{
			if (false == ReverseValue)
			{
				ReverseDir();
				SetGravityPower(-DefaultGravityPower + HoleDefaultGravityValue);
			}
		}
		else
		{
			if (true == ReverseValue)
			{
				ReverseDir();
				SetGravityPower(DefaultGravityPower - HoleDefaultGravityValue);
			}
		}
	}

	// Run의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
			}
			else
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 충돌 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				while (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
				{
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::DOWN);
					RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::UP);
					AddPos(float4::LEFT);
				}
				AddPos(float4::RIGHT);
			}
			else
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

	// Run의 바닥 충돌 체크
	{
		unsigned int LeftDownColor;
		unsigned int MiddleDownColor;
		unsigned int RightDownColor;

		if (true == ReverseValue)
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP);
		}
		else
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN);
		}

		if (LeftDownColor != RGB(255, 0, 0) &&
			MiddleDownColor != RGB(255, 0, 0) &&
			RightDownColor != RGB(255, 0, 0))
		{
			ChangeState(PlayerState::Fall);
			FromJump = false;
			return;
		}
	}
	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// Fall의 중력 반전 체크
	{
		unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		if (MidColor == RGB(0, 255, 255))
		{
			if (false == ReverseValue)
			{
				ReverseDir();
				SetGravityPower(-DefaultGravityPower + HoleDefaultGravityValue);
			}
		}
		else
		{
			if (true == ReverseValue)
			{
				ReverseDir();
				SetGravityPower(DefaultGravityPower - HoleDefaultGravityValue);
			}
		}
	}

	// Fall의 바닥 충돌 체크
	{
		unsigned int LeftDownColor;
		unsigned int MiddleDownColor;
		unsigned int RightDownColor;

		// 중력 반전 체크
		if (true == ReverseValue)
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP - float4::LEFT);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP - float4::RIGHT);

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				while (LeftDownColor != RGB(255, 0, 0) &&
					MiddleDownColor != RGB(255, 0, 0) &&
					RightDownColor != RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP - float4::RIGHT);
					AddPos(float4::DOWN);
				}
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
		else
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN - float4::LEFT);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN - float4::RIGHT);

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				while (LeftDownColor != RGB(255, 0, 0) &&
					MiddleDownColor != RGB(255, 0, 0) &&
					RightDownColor != RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN - float4::RIGHT);
					AddPos(float4::UP);
				}

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
	}

	// Fall의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
			}
			else
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;
			

			// 우측 충돌 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				while (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
				{
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::DOWN);
					RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::UP);
					AddPos(float4::LEFT);
				}
				AddPos(float4::RIGHT);
			}
			else
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}
		}
		AddPos(MovePos);
	}

	// Jump에서 Fall로 바뀐지 체크(Tumbling 애니메이션을 위한 작업)
	if (FromJump == true)
	{
		int CurFrame = static_cast<int>(MainRenderer->GetCurFrame());
		SetAnimation("Tumbling", CurFrame);
		if (true == MainRenderer->IsAnimationEnd())
		{
			FromJump = false;
		}
	}
	else
	{
		SetAnimation("Fall");
	}
}

void Player::JumpUpdate(float _DeltaTime)
{

	// Jump의 상단 충돌 체크
	{
		unsigned int LeftUpColor;
		unsigned int MiddleUpColor;
		unsigned int RightUpColor;
		if (true == ReverseValue)
		{
			LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
			RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);
		}
		else
		{
			LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
			RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
		}

		if (LeftUpColor == RGB(255, 0, 0) ||
			MiddleUpColor == RGB(255, 0, 0) ||
			RightUpColor == RGB(255, 0, 0))
		{
			GravityReset();
			ChangeState(PlayerState::Fall);
			return;
		}
		else
		{
			Gravity(_DeltaTime);
		}
	}

	// 일정 높이 이상 도달 시 Tumbling 애니메이션으로 교체 및 Fall 상태로 변환
	if (true == ReverseValue)
	{
		if (100.0f > GetGravityVector().Y)
		{
			FromJump = true;
			SetAnimation("Tumbling");
			ChangeState(PlayerState::Fall);
			return;
		}
	}
	else
	{
		if (-100.0f < GetGravityVector().Y)
		{
			FromJump = true;
			SetAnimation("Tumbling");
			ChangeState(PlayerState::Fall);
			return;
		}
	}
	

	// Jump의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
			}
			else
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 충돌 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				while (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
				{
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::DOWN);
					RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::UP);
					AddPos(float4::LEFT);
				}
				AddPos(float4::RIGHT);
			}
			else
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