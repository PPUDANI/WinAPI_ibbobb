#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"


void Player::IdleUpdate(float _DeltaTime)
{
	// Run�� �߷� ���� üũ
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

	// Run ���� üũ
	if (true == GameEngineInput::IsPress(MoveLeftKey) || true == GameEngineInput::IsPress(MoveRightKey))
	{
		ChangeState(PlayerState::Run);
		return;
	}
	
	// Jump ���� üũ
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
	
	// Crouch ���� üũ
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		return;
	}

	// Idle�� �ٴ� �浹 üũ
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
	
	// Blink, Idle �ִϸ��̼� ���� ����
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

	// ���� ��ȯ
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::Right;
	}

	// Ű�� ���� Idle ���·� ��ȯ
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
	// Run�� Crouch ���� üũ
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		SetAnimation("Crouch");
		return;
	}

	// Run�� Jump ���� üũ
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

	// Run�� �߷� ���� üũ
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

	// Run�� ��, �� �̵�
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				// ���� ���� ����
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

			// ���� Run�� �� �浹 ����
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

			// ���� �浹 üũ
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				// ���� ���� ����
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

			// ���� Run�� �� �浹 ����
			
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

	// Run�� �ٴ� �浹 üũ
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
	// Fall�� �߷� ���� üũ
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
					// �߷� �Ѱ�ġ �ʱ�ȭ
					if (false == PrevAreaVectorInit)
					{
						PrevAreaVector.Y = GetGravityVector().Y;

						// �߷��� �ʹ� �۾� ���ۿ��� �� �������� ���ɼ� ����
						if (PrevAreaVector.Y < MinGravityInHole)
						{
							PrevAreaVector.Y = MinGravityInHole;
						}

						PrevAreaVectorInit = true;
					}

					// �������� �� �߷��� ��ȭ���� �߷� ���������� �Ѿ��ٸ� "�߷� ����ġ" ����
					if (GetGravityVector().Y > PrevAreaVector.Y * ErrorRangeOfGravity)
					{
						PrevAreaVector.Y = -GetGravityVector().Y;

						// �������� ��ǥ �̻����� �ö� ���ɼ� ����
						if (PrevAreaVector.Y < -MaxGravityInHole)
						{
							PrevAreaVector.Y = -MaxGravityInHole;
						}

						// Vector�� ���� �߷°����� �����ϱ� ���� PrevAreaVector�� ����ȭ �Ͽ����Ƿ� ������ �־���
						SetGravityVector(-PrevAreaVector);
					}
					else
					{
						// �������� ��ǥ �̻����� �ö� ���ɼ� ����
						if (PrevAreaVector.Y > MaxGravityInHole)
						{
							PrevAreaVector.Y = MaxGravityInHole;
						}

						// �������� ���̶�� DeltaTime�� ���ؼ� ���� GravityVector ���� ����
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
					// �ʱ�ȭ
					if (false == PrevAreaVectorInit)
					{
						PrevAreaVector.Y = GetGravityVector().Y;

						// �߷��� �ʹ� �۾� ���ۿ��� �� �������� ���ɼ� ����
						if (PrevAreaVector.Y > -MinGravityInHole)
						{
							PrevAreaVector.Y = -MinGravityInHole;
						}

						PrevAreaVectorInit = true;
					}

					// �������� �� �߷��� ��ȭ���� �߷� ���������� �Ѿ��ٸ� "�߷� ����ġ" ����
					if (GetGravityVector().Y < PrevAreaVector.Y * ErrorRangeOfGravity)
					{
						PrevAreaVector.Y = -GetGravityVector().Y;

						// �������� ��ǥ �̻����� �ö� ���ɼ� ����
						if (PrevAreaVector.Y > MaxGravityInHole)
						{
							PrevAreaVector.Y = MaxGravityInHole;
						}

						SetGravityVector(-PrevAreaVector);
					}
					else
					{
						// �������� ��ǥ �̻����� �ö� ���ɼ� ����
						if (PrevAreaVector.Y < -MaxGravityInHole)
						{
							PrevAreaVector.Y = -MaxGravityInHole;
						}

						// �������� ���̶�� DeltaTime�� ���ؼ� ���� GravityVector ���� ����
						SetGravityVector(PrevAreaVector);
						PrevAreaVector.Y = -GetGravityVector().Y;
					}
				}
			}
		}
	}

	// Fall�� �ٴ� �浹 üũ (�ٴڿ� ���� ����)
	{
		unsigned int LeftDownColor;
		unsigned int MiddleDownColor;
		unsigned int RightDownColor;
		
		// �߷� ���� üũ
		if (true == ReverseValue)
		{
			LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP - float4::LEFT);
			MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
			RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP - float4::RIGHT);
			std::vector<GameEngineCollision*> _ColVec;

			// �ٴ� üũ
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
				// �������� ����
				AddPos(float4::UP);

				// "Fall" ���� ���� ���� �ʱ�ȭ
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// �ٴڿ� ���� �� ����Ȧ ��ȣ�ۿ� �� �߰��� �߷� �ʱ�ȭ
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
				// �������� ����
				AddPos(float4::UP);

				// "Fall" ���� ���� ���� �ʱ�ȭ
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// �ٴڿ� ���� �� ����Ȧ ��ȣ�ۿ� �� �߰��� �߷� �ʱ�ȭ
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

			// �ٴ� üũ
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
				// �������� ����
				AddPos(float4::DOWN);

				// ��ȯ�� �ܺ� ���� �ʱ�ȭ
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// �ٴڿ� ���� �� ����Ȧ ��ȣ�ۿ� �� �߰��� �߷� �ʱ�ȭ
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
				// �������� ����
				AddPos(float4::DOWN);

				// ��ȯ�� �ܺ� ���� �ʱ�ȭ
				GravityReset();
				LengthWorpPass = false;
				WidthWorpPass = false;
				PrevAreaVectorInit = false;

				// �ٴڿ� ���� �� ����Ȧ ��ȣ�ۿ� �� �߰��� �߷� �ʱ�ȭ
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

	// Fall�� ��, �� �̵�
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ

			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT);
			if (LeftUpColor != RGB(255, 0, 0) &&
				LeftMiddleColor != RGB(255, 0, 0) &&
				LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
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

			// ���� �浹 üũ
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT);
			if (RightUpColor != RGB(255, 0, 0) &&
				RightMiddleColor != RGB(255, 0, 0) &&
				RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
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

	// �߷� ���� �� Fall ���¿��� ��� �浹 �������� ���� ��� �浹 üũ
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
	
	// Jump���� Fall�� �ٲ��� üũ(Tumbling �ִϸ��̼��� ���� �۾�)
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
	// Jump�� �߷� ���� üũ
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

	// Jump�� ��� �浹 üũ
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

	// ���� ���� �̻� ���� �� Tumbling �ִϸ��̼����� ��ü �� Fall ���·� ��ȯ
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
	
	// Jump�� ��, �� �̵�
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ

			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT);
			if (LeftUpColor != RGB(255, 0, 0) &&
				LeftMiddleColor != RGB(255, 0, 0) &&
				LeftDownColor != RGB(255, 0, 0))
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
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

			// ���� �浹 üũ
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT);
			if (RightUpColor != RGB(255, 0, 0) &&
				RightMiddleColor != RGB(255, 0, 0) &&
				RightDownColor != RGB(255, 0, 0))
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
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

	// Run�� Crouch ���� üũ
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		SetAnimation("Crouch");
		return;
	}

	// Run�� Jump ���� üũ
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

	// Run�� ��, �� �̵�
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
			{
				// ���� ���� ����
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

			// ���� �浹 üũ
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);
			if (RightUpColor == RGB(255, 0, 0) || RightMiddleColor == RGB(255, 0, 0) || RightDownColor == RGB(255, 0, 0))
			{
				// ���� ���� ����
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