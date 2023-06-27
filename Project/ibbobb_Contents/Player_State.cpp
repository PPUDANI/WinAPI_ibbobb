#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"


void Player::IdleUpdate(float _DeltaTime)
{
	// Idle�� �߷� ���� üũ
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

	// Idle�� �ٴ� �浹 üũ
	if (true == DownToOtherUpCheck())
	{
		DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
		ChangeState(PlayerState::RidingMode);
		return;
	}
	else if (false == DownMapColCheck())
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	// Run ���� üũ
	if (true == GameEngineInput::IsPress(MoveLeftKey) ||
		true == GameEngineInput::IsPress(MoveRightKey))
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

	OtherPlayerMoveCheck();


	// Crouch ���� üũ
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		ChangeState(PlayerState::Crouch);
		return;
	}

	// Blink, Idle �ִϸ��̼� ���� ����
	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);

	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
		AnimIsBlink = true;
		return;
	}
	else if (true == AnimIsBlink)
	{
		if (true == MainRenderer->IsAnimationEnd())
		{
			AnimIsBlink = false;
			SetAnimation("Idle");
		}
	}
	else
	{
		SetAnimation("Idle");
	}
}

void Player::CrouchUpdate(float _DeltaTime)
{
	// Idle�� �߷� ���� üũ
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

	// Idle�� �ٴ� �浹 üũ
	if (true == DownToOtherUpCheck())
	{
		DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
		ChangeState(PlayerState::RidingMode);
		return;
	}
	else if (false == DownMapColCheck())
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	OtherPlayerMoveCheck();

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
		IdleInitFromFall();
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

	// Run�� �ٴ� �浹 üũ
	if (false == DownMapColCheck() &&
		false == DownToOtherBodyCheck())
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	// Run�� ��, �� �̵�
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ

			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// ���¿� ���� �浹 ����
			if(true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();

				if (OtherPlayerState == PlayerState::Idle ||
					OtherPlayerState == PlayerState::Crouch)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
				else
				{
					MovePos = float4::ZERO;
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// ���� �浹 üũ
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// ���¿� ���� �浹 ����
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (OtherPlayerState == PlayerState::Idle ||
					OtherPlayerState == PlayerState::Crouch)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
				else
				{
					MovePos = float4::ZERO;
				}
			}
		}
		else
		{
			IdleInitFromFall();
			ChangeState(PlayerState::Idle);
			return;
		}


		AddPos(MovePos);
	}

	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// Fall�� �߷� ���� üũ
	ReverseInit();
	
	// Fall�� �ٴ� �浹 üũ
	{
		if (true == DownMapColCheck())
		{
			IdleInitFromFall();
			ChangeState(PlayerState::Idle);
			return;
		}
		else if (true == DownToOtherUpCheck() &&
			ReverseValue == OtherPlayer->GetReverseValue())
		{
			if (true == ReverseValue)
			{
				while (true == DownToOtherUpCheck())
				{
					AddPos(float4::DOWN);
				}
				// �������� ����
				AddPos(float4::UP);
			}
			else
			{
				while (true == DownToOtherUpCheck())
				{
					AddPos(float4::UP);
				}
				// �������� ����
				AddPos(float4::DOWN);
			}

			IdleInitFromFall();
			DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
			ChangeState(PlayerState::RidingMode);
			return;
		}
		else if (ReverseValue != OtherPlayer->GetReverseValue() &&
			false == HorizontalWorpPass &&
			true == DownToOtherBodyCheck())
		{

			if (true == ReverseValue)
			{
				while (true == DownToOtherBodyCheck())
				{
					AddPos(float4::DOWN);
				}
				// �������� ����
				AddPos(float4::UP);
			}
			else
			{
				while (true == DownToOtherBodyCheck())
				{
					AddPos(float4::UP);
				}
				// �������� ����
				AddPos(float4::DOWN);
			}


			IdleInitFromFall();
			ChangeState(PlayerState::Idle);
			return;
		}
		else
		{
			Gravity(_DeltaTime);
		}
	}

	// Fall�� ��, �� �̵�
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ
			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
			if (true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Idle == OtherPlayerState ||
					PlayerState::Crouch == OtherPlayerState)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);		
				}
				else
				{
					MovePos = float4::ZERO;
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// ���� �浹 üũ
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Idle == OtherPlayerState ||
					PlayerState::Crouch == OtherPlayerState)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
				else
				{
					MovePos = float4::ZERO;
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
	if (true == UpMapColCheck())
	{
		GravityReset();
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
	// 'Jump'���¿��� �߷��� �����Ǹ� 'Fall'���·� ��ȯ
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
	if (true == UpMapColCheck())
	{
		GravityReset();
		ChangeState(PlayerState::Fall);
	}
	else if (
		true == UpToOtherBodyCheck() &&
		PlayerState::RidingMode != OtherPlayer->GetState())
	{
		GravityReset();
		ChangeState(PlayerState::Fall);
	}
	else
	{
		Gravity(_DeltaTime);
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
			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
			if (true == LeftToOtherRightCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Idle == OtherPlayerState ||
					PlayerState::Crouch == OtherPlayerState)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
				else
				{
					MovePos = float4::ZERO;
				}
			}
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// ���� �浹 üũ
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// ���� Run�� �� �浹 ����
			if (true == RightToOtherLeftCheck())
			{
				PlayerState OtherPlayerState = OtherPlayer->GetState();
				if (PlayerState::Idle == OtherPlayerState ||
					PlayerState::Crouch == OtherPlayerState)
				{
					MovePos *= 0.6f;
					OtherPlayer->AddPos(MovePos);
				}
				else
				{
					MovePos = float4::ZERO;
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
	// RidingMode�� �߷� ���� üũ
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

	// Jump ���� üũ
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


	// RideMode ���� �ڵ�
	if (OtherPlayer->GetState() != PlayerState::Idle &&
		OtherPlayer->GetState() != PlayerState::Crouch)
	{
		if (false == LeftMapColCheck() || false == RightMapColCheck())
		{
			SetPos(OtherPlayer->GetPos() + DistanceFromOtherPlayer);
		}
	}

	// Crouch ���� üũ
	if (true == GameEngineInput::IsPress(CrouchKey))
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;
		}
		SetAnimation("Crouch");
		return;
	}

	// ��� �浹�� �Ͼ��� OtherPlayer ����
	if (true == UpMapColCheck())
	{
		if (true == ReverseValue)
		{
			AddPos(float4::UP);
			OtherPlayer->AddPos(float4::UP);
		}
		else
		{
			AddPos(float4::DOWN);
			OtherPlayer->AddPos(float4::DOWN);
		}

		OtherPlayer->GravityReset();
		OtherPlayer->ChangeState(PlayerState::Fall);
		return;
	}


	// �Ӹ������� ��, �� �̵� �� �� ���������� ���� ����
	if (false == DownToOtherUpCheck())
	{
		if (PlayerDir::Left == CurDir)
		{
			AddPos(float4::LEFT * 5);
			LeftMapColCheck();
		}
		else
		{
			AddPos(float4::RIGHT * 5);
			RightMapColCheck();
		}

		ChangeState(PlayerState::Fall);
		return;
	}
	else if (true == DownMapColCheck())
	{
		if (false == LeftMapColCheck() ||
			false == RightMapColCheck())
		{
			IdleInitFromFall();
			ChangeState(PlayerState::Idle);
		}
		return;
	}


	// ��, �� �̵�
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// ���� �浹 üũ
			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			DistanceFromOtherPlayer += MovePos;
			AddPos(MovePos);

			SetAnimation("Run");
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// ���� �浹 üũ
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
				
			}

			DistanceFromOtherPlayer += MovePos;
			AddPos(MovePos);

			SetAnimation("Run");
		}
		else
		{
			MovePos = float4::ZERO;
			// Blink, Idle �ִϸ��̼� ���� ����
			int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 600);

			if (RandomNumber == 1)
			{
				SetAnimation("Blink");
				AnimIsBlink = true;
				return;
			}

			if (true == AnimIsBlink)
			{
				if (true == MainRenderer->IsAnimationEnd())
				{
					AnimIsBlink = false;
					SetAnimation("Idle");
				}
			}
			else
			{
				SetAnimation("Idle");
			}
		}
	}




}

void Player::DeadUpdate(float _DeltaTime)
{

	if (true == MainRenderer->IsAnimationEnd())
	{
		Death();
	}
}