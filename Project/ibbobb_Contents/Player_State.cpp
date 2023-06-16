#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"


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
			SetGravityVector(float4::DOWN * JumpForce);
		}
		else
		{
			SetGravityVector(float4::UP * JumpForce);
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
	// 방향 전환
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
			SetGravityVector(float4::DOWN * JumpForce);
		}
		else
		{
			SetGravityVector(float4::UP * JumpForce);
		}

		FromJump = false;
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
				FromRun = true;
				ReverseValue = true;
				SetGravityPower(-DefaultGravityPower);
			}
		}
		else if(MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				FromRun = true;
				ReverseValue = false;
				SetGravityPower(DefaultGravityPower);
			}
		}
	}

	// Run의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크 (벽에 박힘 제거)
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
				// 지글현상 제거
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

			// 우측 충돌 체크 (벽에 박힘 제거)
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				while (RightUpColor == RGB(255, 0, 0) ||
					RightMiddleColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
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
				ReverseValue = true;
				SetGravityPower(-DefaultGravityPower);

				// Run 상태에서 세로워프를 타고 Fall이 된 경우가 아닐때만 시행
				if (false == FromRun)
				{
					// 워프 통과 시 떨어진 만큼 이동하는 거리 제한
					SetGravityLimit(GetGravityVector().Y);

					// 워프홀 통과 시 못빠져 나올 가능성 제거
					float AbsoluteVector = GetGravityVector().Y;
					if (0 > AbsoluteVector)
					{
						AbsoluteVector = -AbsoluteVector; // 절대값 변환
					}

					if (DefaultGravityPower * 0.3f >= AbsoluteVector)
					{
						SetGravityPower(-DefaultGravityPower + HoleDefaultGravityValue);
					}
					else
					{
						SetGravityPower(-DefaultGravityPower);
					}
				}
			}
			else
			{
				ResetGravityLimit();
			}
		}
		else if (MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				ReverseValue = false;
				SetGravityPower(DefaultGravityPower);

				// Run 상태에서 세로워프를 타고 Fall이 된 경우가 아닐때만 시행
				if (false == FromRun)
				{
					// 워프 통과 시 중력으로 이동하는 거리 제한
					SetGravityLimit(-GetGravityVector().Y);

					// 워프홀 통과 시 못빠져 나올 가능성 제거
					float AbsoluteVector = GetGravityVector().Y;
					if (0 > AbsoluteVector)
					{
						AbsoluteVector = -AbsoluteVector; // 절대값 변환
					}

					if (DefaultGravityPower * 0.3f >= AbsoluteVector)
					{
						SetGravityPower(DefaultGravityPower - HoleDefaultGravityValue);
					}
					else
					{
						SetGravityPower(DefaultGravityPower);
					}
				}
			}
			else
			{
				ResetGravityLimit();
			}
		}
	}

	// Fall의 바닥 충돌 체크 (바닥에 박힘 제거)
	{
		unsigned int LeftDownColor;
		unsigned int MiddleDownColor;
		unsigned int RightDownColor;

		unsigned int MiddleColor;
		unsigned int MiddleUpColor;


		// 중력 반전 체크
		if (true == ReverseValue)
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP - float4::LEFT);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP - float4::RIGHT);
			MiddleColor = GetGroundColor(RGB(255, 0, 0), float4::ZERO);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0) ||
				MiddleColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0))
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0) ||
					MiddleColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
					MiddleColor = GetGroundColor(RGB(255, 0, 0), float4::ZERO);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);

					AddPos(float4::DOWN);
				}
				// 지글현상 제거
				AddPos(float4::UP);

				// 변환한 중력 설정 초기화
				ResetGravityLimit();
				GravityReset();
				FromRun = false;

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
			MiddleColor = GetGroundColor(RGB(255, 0, 0), float4::ZERO);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0) ||
				MiddleColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0))
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0) ||
					MiddleColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);
					MiddleColor = GetGroundColor(RGB(255, 0, 0), float4::ZERO);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);

					AddPos(float4::UP);
				}
				// 지글현상 제거
				AddPos(float4::DOWN);

				// 변환한 외부 설정 초기화
				ResetGravityLimit();
				GravityReset();
				FromRun = false;

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
			if (true == ReverseValue)
			{
				unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT);
				unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);

				if (LeftUpColor == RGB(255, 0, 0) ||
					LeftMiddleColor == RGB(255, 0, 0))
				{
					while (LeftUpColor == RGB(255, 0, 0) ||
						LeftMiddleColor == RGB(255, 0, 0))
					{
						LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
						LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
						AddPos(float4::RIGHT);
					}
					// 지글현상 제거
					AddPos(float4::LEFT);
				}
				else
				{
					MovePos = { -Speed * _DeltaTime, 0.0f };
				}
			}
			else
			{
				unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT);
				unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);

				if (LeftUpColor == RGB(255, 0, 0) ||
					LeftMiddleColor == RGB(255, 0, 0))
				{
					while (LeftUpColor == RGB(255, 0, 0) ||
						LeftMiddleColor == RGB(255, 0, 0))
					{
						LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
						LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
						AddPos(float4::RIGHT);
					}
					// 지글현상 제거
					AddPos(float4::LEFT);
				}
				else
				{
					MovePos = { -Speed * _DeltaTime, 0.0f };
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;
			if (true == ReverseValue)
			{
				// 우측 충돌 체크
				unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT);
				unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);

				if (RightUpColor == RGB(255, 0, 0) ||
					RightMiddleColor == RGB(255, 0, 0))
				{
					while (RightUpColor == RGB(255, 0, 0) ||
						RightMiddleColor == RGB(255, 0, 0))
					{
						RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
						RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
						AddPos(float4::LEFT);
					}
					// 지글현상 제거
					AddPos(float4::RIGHT);
				}
				else
				{
					MovePos = { Speed * _DeltaTime, 0.0f };
				}
			}
			else
			{
				// 우측 충돌 체크
				unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT);
				unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);

				if (RightUpColor == RGB(255, 0, 0) ||
					RightMiddleColor == RGB(255, 0, 0))
				{
					while (RightUpColor == RGB(255, 0, 0) ||
						RightMiddleColor == RGB(255, 0, 0))
					{
						RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
						RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
						AddPos(float4::LEFT);
					}
					// 지글현상 제거
					AddPos(float4::RIGHT);
				}
				else
				{
					MovePos = { Speed * _DeltaTime, 0.0f };
				}
			}
			
		}
		AddPos(MovePos);
	}

	

	

	// 중력 반전 시 Fall 상태에서 상단 충돌 현상으로 인한 상단 충돌 체크
	{
		unsigned int LeftUpColor;
		unsigned int MiddleUpColor;
		unsigned int RightUpColor;
		if (true == ReverseValue)
		{
			LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN - float4::LEFT);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
			RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN - float4::RIGHT);

			if (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0) ||
					RightUpColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);
					AddPos(float4::UP);
				}
				GravityReset();
			}
		}
		else
		{
			LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP - float4::LEFT);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
			RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP - float4::RIGHT);

			if (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0) ||
					RightUpColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
					AddPos(float4::DOWN);
				}
				GravityReset();
			}
		}
	}
	
	// Jump에서 Fall로 바뀐지 체크(Tumbling 애니메이션을 위한 작업)
	{
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

			if (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0) ||
					RightUpColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);
					AddPos(float4::UP);
				}
				GravityReset();
				ChangeState(PlayerState::Fall);
				return;
			}
			else
			{
				Gravity(_DeltaTime);
			}
		}
		else
		{
			LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
			MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
			RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
			if (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				while (LeftUpColor == RGB(255, 0, 0) ||
					MiddleUpColor == RGB(255, 0, 0) ||
					RightUpColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
					MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
					AddPos(float4::DOWN);
				}
				GravityReset();
				ChangeState(PlayerState::Fall);
				return;
			}
			else
			{
				Gravity(_DeltaTime);
			}
		}
	}

	// 일정 높이 이상 도달 시 Tumbling 애니메이션으로 교체 및 Fall 상태로 변환
	if (true == ReverseValue)
	{
		if (JumpForce / 2.0f > GetGravityVector().Y)
		{
			FromJump = true;
			SetAnimation("Tumbling");
			ChangeState(PlayerState::Fall);
			return;
		}
	}
	else
	{
		if (-JumpForce / 2.0f < GetGravityVector().Y)
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

			// 좌측 충돌 체크 (벽에 박힘 제거)
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

			// 우측 충돌 체크 (벽에 박힘 제거)
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