#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"


void Player::IdleUpdate(float _DeltaTime)
{
	// Run의 중력 반전 체크
	{
		unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		if (MidColor == RGB(0, 255, 255))
		{
			if (false == ReverseValue)
			{
				ChangeState(PlayerState::Fall);
				return;
			}
		}
		else if (MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				ChangeState(PlayerState::Fall);
				return;
			}
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

	// Idle의 바닥 충돌 체크
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

	OtherPlayerMoveCheck();
	
	// Blink, Idle 애니메이션 랜덤 교차
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);

	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
		return;
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
		SetAnimation("Idle");
		ChangeState(PlayerState::Idle);
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
				ChangeState(PlayerState::Fall);
				return;
			}
		}
		else if(MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				ChangeState(PlayerState::Fall);
				return;
			}
		}
	}

	// Run의 좌, 우 이동
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				// 벽에 박힘 제거
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
				MovePos = float4::ZERO;
			}
			else
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			if(true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Run == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
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
				// 벽에 박힘 제거
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
				MovePos = float4::ZERO;
			}
			else
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Run == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
			}
		}
		else
		{
			MovePos = float4::ZERO;
			SetAnimation("Idle");
			ChangeState(PlayerState::Idle);
			return;
		}

		//if (true == UpToOtherDownCheck())
		//{
		//	OtherPlayer->AddPos(MovePos);
		//}

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
			RightDownColor != RGB(255, 0, 0) &&
			false == DownToOtherUpCheck())
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
				ReverseCol();
				ReverseValue = true;
				SetGravityPower(-DefaultGravityPower);

				if (true == LengthWorpPass)
				{
					// 중력 한계치 초기화
					if (false == PrevAreaVectorInit)
					{
						PrevAreaVector.Y = GetGravityVector().Y;

						// 중력이 너무 작아 구멍에서 못 빠져나올 가능성 제거
						if (PrevAreaVector.Y < MinGravityInHole)
						{
							PrevAreaVector.Y = MinGravityInHole;
						}

						PrevAreaVectorInit = true;
					}

					// 영역반전 시 중력의 변화량이 중력 오차범위를 넘었다면 "중력 기준치" 변경
					if (GetGravityVector().Y > PrevAreaVector.Y * ErrorRangeOfGravity)
					{
						PrevAreaVector.Y = -GetGravityVector().Y;

						// 레벨기준 좌표 이상으로 올라갈 가능성 제거
						if (PrevAreaVector.Y < -MaxGravityInHole)
						{
							PrevAreaVector.Y = -MaxGravityInHole;
						}

						// Vector를 이전 중력값으로 변경하기 전에 PrevAreaVector를 음수화 하였으므로 음수로 넣어줌
						SetGravityVector(-PrevAreaVector);
					}
					else
					{
						// 레벨기준 좌표 이상으로 올라갈 가능성 제거
						if (PrevAreaVector.Y > MaxGravityInHole)
						{
							PrevAreaVector.Y = MaxGravityInHole;
						}

						// 오차범위 안이라면 DeltaTime로 인해서 생긴 GravityVector 오차 제거
						SetGravityVector(PrevAreaVector);
						PrevAreaVector.Y = -GetGravityVector().Y;
					}
				}
			}
		}
		else if (MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				ReverseCol();

				ReverseValue = false;
				SetGravityPower(DefaultGravityPower);

				if (true == LengthWorpPass)
				{
					// 초기화
					if (false == PrevAreaVectorInit)
					{
						PrevAreaVector.Y = GetGravityVector().Y;

						// 중력이 너무 작아 구멍에서 못 빠져나올 가능성 제거
						if (PrevAreaVector.Y > -MinGravityInHole)
						{
							PrevAreaVector.Y = -MinGravityInHole;
						}

						PrevAreaVectorInit = true;
					}

					// 영역반전 시 중력의 변화량이 중력 오차범위를 넘었다면 "중력 기준치" 변경
					if (GetGravityVector().Y < PrevAreaVector.Y * ErrorRangeOfGravity)
					{
						PrevAreaVector.Y = -GetGravityVector().Y;

						// 레벨기준 좌표 이상으로 올라갈 가능성 제거
						if (PrevAreaVector.Y > MaxGravityInHole)
						{
							PrevAreaVector.Y = MaxGravityInHole;
						}

						SetGravityVector(-PrevAreaVector);
					}
					else
					{
						// 레벨기준 좌표 이상으로 올라갈 가능성 제거
						if (PrevAreaVector.Y < -MaxGravityInHole)
						{
							PrevAreaVector.Y = -MaxGravityInHole;
						}

						// 오차범위 안이라면 DeltaTime로 인해서 생긴 GravityVector 오차 제거
						SetGravityVector(PrevAreaVector);
						PrevAreaVector.Y = -GetGravityVector().Y;
					}
				}
			}
		}
	}

	// Fall의 바닥 충돌 체크 (바닥에 박힘 제거)
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
			std::vector<GameEngineCollision*> _ColVec;

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);

					AddPos(float4::DOWN);
				}
				// 지글현상 제거
				AddPos(float4::UP);

				// "Fall" 상태 연산 설정 초기화
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// 바닥에 닿을 시 워프홀 상호작용 때 추가된 중력 초기화
				SetGravityPower(-DefaultGravityPower);

				MovePos = float4::ZERO;
				SetAnimation("Idle");
				ChangeState(PlayerState::Idle);
				return;
			}
			else if (true == DownToOtherUpCheck())
			{
				while (true == DownToOtherUpCheck())
				{
					AddPos(float4::DOWN);
				}
				// 지글현상 제거
				AddPos(float4::UP);

				// "Fall" 상태 연산 설정 초기화
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// 바닥에 닿을 시 워프홀 상호작용 때 추가된 중력 초기화
				SetGravityPower(-DefaultGravityPower);

				MovePos = float4::ZERO;
				DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
				SetAnimation("Idle");
				ChangeState(PlayerState::RidingMode);
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
			std::vector<GameEngineCollision*> _ColVec;

			// 바닥 체크
			if (LeftDownColor == RGB(255, 0, 0) ||
				MiddleDownColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);

					AddPos(float4::UP);
				}
				// 지글현상 제거
				AddPos(float4::DOWN);

				// 변환한 외부 설정 초기화
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// 바닥에 닿을 시 워프홀 상호작용 때 추가된 중력 초기화
				SetGravityPower(DefaultGravityPower);

				MovePos = float4::ZERO;
				SetAnimation("Idle");
				ChangeState(PlayerState::Idle);
				return;
			}
			else if (true == DownToOtherUpCheck())
			{
				while (true == DownToOtherUpCheck())
				{
					AddPos(float4::UP);
				}
				// 지글현상 제거
				AddPos(float4::DOWN);

				// 변환한 외부 설정 초기화
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// 바닥에 닿을 시 워프홀 상호작용 때 추가된 중력 초기화
				SetGravityPower(DefaultGravityPower);

				MovePos = float4::ZERO;
				DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
				SetAnimation("Idle");
				ChangeState(PlayerState::RidingMode);
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

			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT);
			if (LeftUpColor != RGB(255, 0, 0) &&
				LeftMiddleColor != RGB(255, 0, 0) &&
				LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			if (true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Fall == OtherPlayerState ||
					PlayerState::Jump == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 충돌 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT);
			if (RightUpColor != RGB(255, 0, 0) &&
				RightMiddleColor != RGB(255, 0, 0) &&
				RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Fall == OtherPlayerState ||
					PlayerState::Jump == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
			}
		}

		if (true == UpToOtherDownCheck())
		{
			OtherPlayer->AddPos(MovePos);
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
	// Jump의 중력 반전 체크
	{
		unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		if (MidColor == RGB(0, 255, 255))
		{
			if (false == ReverseValue)
			{
				ChangeState(PlayerState::Fall);
				return;
			}
		}
		else if (MidColor == RGB(255, 255, 255))
		{
			if (true == ReverseValue)
			{
				ChangeState(PlayerState::Fall);
				return;
			}
		}
	}

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

			// 좌측 충돌 체크

			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT);
			if (LeftUpColor != RGB(255, 0, 0) &&
				LeftMiddleColor != RGB(255, 0, 0) &&
				LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			if (true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Fall == OtherPlayerState ||
					PlayerState::Jump == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// 우측 충돌 체크
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT);
			if (RightUpColor != RGB(255, 0, 0) &&
				RightMiddleColor != RGB(255, 0, 0) &&
				RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Fall == OtherPlayerState ||
					PlayerState::Jump == OtherPlayerState)
				{
					MovePos = float4::ZERO;
				}
				else
				{
					MovePos = MovePos * 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
			}
		}

		if (true == UpToOtherDownCheck())
		{
			OtherPlayer->AddPos(MovePos);
		}
		AddPos(MovePos);
	}

	SetAnimation("Jump");
}

void Player::RidingModeUpdate(float _DeltaTime)
{
	if (OtherPlayer->GetState() != PlayerState::Idle)
	{
	    SetPos(OtherPlayer->GetPos() + DistanceFromOtherPlayer);
	}

	if (false == DownToOtherUpCheck())
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	// Run의 Crouch 상태 체크
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
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

	// Run의 좌, 우 이동
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				// 벽에 박힘 제거
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
				MovePos = float4::ZERO;
			}
			else
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}
			SetAnimation("Run");
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
				// 벽에 박힘 제거
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
				MovePos = float4::ZERO;
			}
			else
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}
			SetAnimation("Run");
		}
		else
		{
			MovePos = float4::ZERO;
			SetAnimation("Idle");

			return;
		}
		DistanceFromOtherPlayer += MovePos;
		AddPos(MovePos);
	}
}

void Player::DeadUpdate(float _DeltaTime)
{
	SetAnimation("Dead");
	if (true == MainRenderer->IsAnimationEnd())
	{
		Death();
	}
}