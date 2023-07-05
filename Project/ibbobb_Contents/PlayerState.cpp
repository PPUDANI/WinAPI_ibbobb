#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"


void Player::IdleUpdate(float _DeltaTime)
{
	// Idle의 중력 반전 체크
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

	// Idle의 바닥 충돌 체크
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


	// Run 상태 체크
	if (true == GameEngineInput::IsPress(MoveLeftKey) ||
		true == GameEngineInput::IsPress(MoveRightKey))
	{
		ChangeState(PlayerState::Run);
		return;
	}
	
	// Jump 상태 체크
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


	// Crouch 상태 체크
	if (false == UpToOtherDownCheck() &&
		true == GameEngineInput::IsPress(CrouchKey))
	{
		CrouchSoundPlay();
		ChangeState(PlayerState::Crouch);
		return;
	}

	// Blink, Idle 애니메이션 랜덤 교차
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
	// Crouch의 중력 반전 체크
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

	// Crouch의 바닥 충돌 체크
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
		IdleInitFromFall();
		ChangeState(PlayerState::Idle);
	}
}



void Player::RunUpdate(float _DeltaTime)
{
	// Run의 Crouch 상태 체크
	if (false == UpToOtherDownCheck() &&
		true == GameEngineInput::IsPress(CrouchKey))
	{
		CrouchSoundPlay();
		ChangeState(PlayerState::Crouch);
		SetAnimation("Crouch");
		return;
	}

	// Run의 Jump 상태 체크
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

	// Run의 바닥 충돌 체크

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

	// Run의 좌, 우 이동
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크

			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 상태에 따른 충돌 제어
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

			// 우측 충돌 체크
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 상태에 따른 충돌 제어
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
	// Fall의 중력 반전 체크
	ReverseInit();
	// Fall의 바닥 충돌 체크
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
				// 지글현상 제거
				AddPos(float4::UP);
			}
			else
			{
				while (true == DownToOtherUpCheck())
				{
					AddPos(float4::UP);
				}
				// 지글현상 제거
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
				// 지글현상 제거
				AddPos(float4::UP);
			}
			else
			{
				while (true == DownToOtherBodyCheck())
				{
					AddPos(float4::UP);
				}
				// 지글현상 제거
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

	// Fall의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
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

			// 우측 충돌 체크
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
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

	// 중력 반전 시 Fall 상태에서 상단 충돌 현상으로 인한 상단 충돌 체크
	if (true == UpMapColCheck())
	{
		GravityReset();
	}
	
	//// Jump에서 Fall로 바뀐지 체크(Tumbling 애니메이션을 위한 작업)
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

	// 'Jump'상태에서 중력이 반전되면 'Fall'상태로 전환
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

	// 일정 높이 이상 도달 시 Tumbling 애니메이션으로 교체 및 Fall 상태로 변환
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
	
	// Jump의 좌, 우 이동
	{
		float4 MovePos = float4::ZERO;

		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
			if (false == LeftMapColCheck())
			{
				MovePos = { -Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
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

			// 우측 충돌 체크
			if (false == RightMapColCheck())
			{
				MovePos = { Speed * _DeltaTime, 0.0f };
			}

			// 서로 Run일 때 충돌 제어
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
	// RidingMode의 중력 반전 체크
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

	// RidingMode의 OtherPlayer중력 반전 체크
	{
		if (OtherPlayer->GetReverseValue() != ReverseValue)
		{
			ChangeState(PlayerState::Fall);
			return;
		}
	}

	// Jump 상태 체크
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


	// RideMode 연산 코드
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

	// 머리위에서 좌, 우 이동 중 못 빠져나오는 현상 제거
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

	// 상단 충돌이 일어날경우 OtherPlayer 제어
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

	// Crouch 상태 체크
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

	// 좌, 우 이동
	{
		if (true == GameEngineInput::IsPress(MoveLeftKey))
		{
			CurDir = PlayerDir::Left;

			// 좌측 충돌 체크
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

			// 우측 충돌 체크
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
			// Blink, Idle 애니메이션 랜덤 교차
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