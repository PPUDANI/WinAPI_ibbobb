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
		if (true == ReverseValue)
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::DOWN);
			}
			AddPos(float4::UP);
		}
		else
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::UP);
			}
			AddPos(float4::DOWN);
		}

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
	if (true == GameEngineInput::IsPress(JumpKey) &&
		false == DownToGravityPlatformCheck())
	{
		if (true == ReverseValue)
		{
			SetGravityVector(float4::DOWN * JumpForce);
		}
		else
		{
			SetGravityVector(float4::UP * JumpForce);
		}
		JumpSoundPlay();
		ChangeState(PlayerState::Jump);
		return;
	}

	OtherPlayerPushCheck();


	// Crouch ���� üũ
	if (false == UpToOtherDownCheck() &&
		true == GameEngineInput::IsPress(CrouchKey))
	{
		CrouchSoundPlay();
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
	// Crouch�� �߷� ���� üũ
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

	// Crouch�� �ٴ� �浹 üũ
	if (true == DownToOtherUpCheck())
	{
		if (true == ReverseValue)
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::DOWN);
			}
			AddPos(float4::UP);
		}
		else
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::UP);
			}
			AddPos(float4::DOWN);
		}

		DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
		ChangeState(PlayerState::RidingMode);
		return;
	}
	else if (false == DownMapColCheck())
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	OtherPlayerPushCheck();

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
	if (false == UpToOtherDownCheck() &&
		true == GameEngineInput::IsPress(CrouchKey))
	{
		CrouchSoundPlay();
		ChangeState(PlayerState::Crouch);
		SetAnimation("Crouch");
		return;
	}

	// Run�� Jump ���� üũ
	if (true == GameEngineInput::IsDown(JumpKey) &&
		false == DownToGravityPlatformCheck())
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
		JumpSoundPlay();
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

	if (true == DownToOtherUpCheck())
	{
		if (true == ReverseValue)
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::DOWN);
			}
			AddPos(float4::UP);
		}
		else
		{
			while (false != DownToOtherUpCheck())
			{
				AddPos(float4::UP);
			}
			AddPos(float4::DOWN);
		}

		DistanceFromOtherPlayer = GetPos() - OtherPlayer->GetPos();
		ChangeState(PlayerState::RidingMode);
		return;
	}
	else if (false == DownMapColCheck())
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
	
	//// Jump���� Fall�� �ٲ��� üũ(Tumbling �ִϸ��̼��� ���� �۾�)
	//{
	//	if (FromJump == true)
	//	{
	//		int CurFrame = static_cast<int>(MainRenderer->GetCurFrame());
	//		SetAnimation("Tumbling", CurFrame);
	//		if (true == MainRenderer->IsAnimationEnd())
	//		{
	//			FromJump = false;
	//		}
	//	}
	//	else
	//	{
	//		
	//	}
	//}
	SetAnimation("Fall");
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
	else if (true == UpToOtherBodyCheck() &&
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
		if (0.0f > GetGravityVector().Y)
		{
			FromJump = true;
			//SetAnimation("Tumbling");
			SetAnimation("Fall");
			ChangeState(PlayerState::Fall);
			return;
		}
	}
	else
	{
		if (0.0f < GetGravityVector().Y)
		{
			FromJump = true;
			//SetAnimation("Tumbling");
			SetAnimation("Fall");
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

	// RidingMode�� OtherPlayer�߷� ���� üũ
	{
		if (OtherPlayer->GetReverseValue() != ReverseValue)
		{
			ChangeState(PlayerState::Fall);
			return;
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
		JumpSoundPlay();
		ChangeState(PlayerState::Jump);
		return;
	}


	// RideMode ���� �ڵ�
	if (OtherPlayer->GetState() != PlayerState::Idle &&
		OtherPlayer->GetState() != PlayerState::Crouch)
	{
		PlayerDir OtherPlayerDir = OtherPlayer->GetDir();
		if (true == LeftMapColCheck())
		{
			AddPos(float4::RIGHT);
			DistanceFromOtherPlayer.X = GetPos().X - OtherPlayer->GetPos().X;
		}

		if (true == RightMapColCheck())
		{
			AddPos(float4::LEFT);
			DistanceFromOtherPlayer.X = GetPos().X - OtherPlayer->GetPos().X;
		}

		SetPos(OtherPlayer->GetPos() + DistanceFromOtherPlayer);
	}

	// �Ӹ������� ��, �� �̵� �� �� ���������� ���� ����
    if (true == DownMapColCheck())
	{
		bool LeftCheck = LeftMapColCheck();
		bool RightCheck = RightMapColCheck();
		if (false == LeftCheck &&
			false == RightCheck)
		{
			IdleInitFromFall();
			ChangeState(PlayerState::Idle);
			return;
		}
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

	// Crouch ���� üũ
	if (true == GameEngineInput::IsDown(CrouchKey))
	{
		CrouchSoundPlay();
	}
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
		AnimIsBlink = false;
		
		SetAnimation("Crouch");
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
				DistanceFromOtherPlayer += MovePos;
			}
			else
			{
				DistanceFromOtherPlayer.X = GetPos().X - OtherPlayer->GetPos().X;
			}

			AddPos(MovePos);
			AnimIsBlink = false;
			SetAnimation("Run");

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
		}
		else if (true == GameEngineInput::IsPress(MoveRightKey))
		{
			CurDir = PlayerDir::Right;

			// ���� �浹 üũ
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
				DistanceFromOtherPlayer += MovePos;
			}
			else
			{
				DistanceFromOtherPlayer.X = GetPos().X - OtherPlayer->GetPos().X;
			}

			AddPos(MovePos);
			AnimIsBlink = false;
			SetAnimation("Run");

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
		Off();
	}
}