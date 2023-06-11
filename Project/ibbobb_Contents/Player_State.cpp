#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

void Player::IdleUpdate(float _DeltaTime)
{
	// Run ���� Ȯ��
	// Idle ���·� �ٲ� �� ����Ű�� ������ ���� �� Run���� �ٲ�� �ְ� �ϱ����� IsPress�� Ű üũ
	if (true == GameEngineInput::IsPress(MoveLeftKey) || true == GameEngineInput::IsPress(MoveRightKey))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump ���� üũ
	if (true == GameEngineInput::IsDown(JumpKey))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	int RandomNumber = GameEngineRandom::MainRandom.RandomInt(1, 400);
	if (RandomNumber == 1)
	{
		SetAnimation("Blink");
	}
	if (true == MainRenderer->IsAnimationEnd())
	{
		SetAnimation("Idle");
	}
}


void Player::RunUpdate(float _DeltaTime)
{
	// ��, �� �̵�
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::UpLeft;

		unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 0, 0), MapLeftCenterCheck);
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
		if (RGB(255, 0, 0) != LeftUpColor &&
			RGB(255, 0, 0) != LeftCenterColor &&
			RGB(255, 0, 0) != LeftDownColor)
		{
			MovePos = { -Speed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
		unsigned int RightCenterColor = GetGroundColor(RGB(255, 0, 0), MapRightCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
		if (RGB(255, 0, 0) != RightUpColor &&
			RGB(255, 0, 0) != RightCenterColor &&
			RGB(255, 0, 0) != RightDownColor)
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


	// �̵��� ����
	if (true == GameEngineInput::IsDown(JumpKey))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// ����, �ٴ� üũ
	unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN * 2);
	unsigned int DownCneterColor = GetGroundColor(RGB(255, 0, 0), MapDownCenterCheck + float4::DOWN * 2);
	unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN * 2);

	if (RGB(255, 0, 0) != LeftDownColor &&
		RGB(255, 0, 0) != DownCneterColor &&
		RGB(255, 0, 0) != RightDownColor)
	{
		ChangeState(PlayerState::Fall);
		return;
	}

	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// ����, �ٴ� üũ
	unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT * 3);
	unsigned int DownCneterColor = GetGroundColor(RGB(255, 0, 0), MapDownCenterCheck);
	unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT * 3);

	if (RGB(255, 0, 0) != LeftDownColor &&
		RGB(255, 0, 0) != DownCneterColor &&
		RGB(255, 0, 0) != RightDownColor)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		unsigned int DownCenterColor = GetGroundColor(RGB(255, 0, 0), MapDownCenterCheck + float4::UP);
		while (DownCenterColor == RGB(255, 0, 0))
		{
			DownCenterColor = GetGroundColor(RGB(255, 0, 0), MapDownCenterCheck + float4::UP);
			AddPos(float4::UP);
		}
		AddPos(float4::UP);
		GravityReset();
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
		return;
	}

	// ��, �� �̵�
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::UpLeft;

		unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 0, 0), MapLeftCenterCheck);
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
		if (RGB(255, 0, 0) != LeftUpColor &&
			RGB(255, 0, 0) != LeftCenterColor &&
			RGB(255, 0, 0) != LeftDownColor)
		{
			MovePos = { -Speed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
		unsigned int RightCenterColor = GetGroundColor(RGB(255, 0, 0), MapRightCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
		if (RGB(255, 0, 0) != RightUpColor &&
			RGB(255, 0, 0) != RightCenterColor &&
			RGB(255, 0, 0) != RightDownColor)
		{
			MovePos = { Speed * _DeltaTime, 0.0f };
		}
	}
	AddPos(MovePos);
	SetAnimation("Fall");
}


void Player::JumpUpdate(float _DeltaTime)
{
	float4 PastPos = GetPos();
	PlayerDir PastDir = CurDir;

	Gravity(_DeltaTime);

	// ��� �浹 üũ
	unsigned int LeftUpColor = GetGroundColor(RGB(255, 255, 255), MapLeftUpCheck - float4::LEFT * 2);
	unsigned int UpCenterColor = GetGroundColor(RGB(255, 255, 255), MapUpCenterCheck);
	unsigned int RightUpColor = GetGroundColor(RGB(255, 255, 255), MapRightUpCheck - float4::RIGHT * 2);

	if (RGB(255, 255, 255) != LeftUpColor ||
		RGB(255, 255, 255) != UpCenterColor ||
		RGB(255, 255, 255) != RightUpColor)
	{
		while (RGB(255, 255, 255) != UpCenterColor)
		{
			UpCenterColor = GetGroundColor(RGB(255, 255, 255), MapUpCenterCheck);
			AddPos(float4::DOWN);
		}
		GravityReset();
		ChangeState(PlayerState::Fall);
	}

	// ��, �� �̵�
	// ��, �� �̵�
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress(MoveLeftKey))
	{
		CurDir = PlayerDir::UpLeft;

		unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck);
		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 0, 0), MapLeftCenterCheck);
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck);
		if (RGB(255, 0, 0) != LeftUpColor &&
			RGB(255, 0, 0) != LeftCenterColor &&
			RGB(255, 0, 0) != LeftDownColor)
		{
			MovePos = { -Speed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress(MoveRightKey))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck);
		unsigned int RightCenterColor = GetGroundColor(RGB(255, 0, 0), MapRightCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck);
		if (RGB(255, 0, 0) != RightUpColor &&
			RGB(255, 0, 0) != RightCenterColor &&
			RGB(255, 0, 0) != RightDownColor)
		{
			MovePos = { Speed * _DeltaTime, 0.0f };
		}
	}
	AddPos(MovePos);

	// ���� �� ���� �ӵ����� ������ �� "Tumbling" ���·� ������Ʈ

	float GravityForce = GetGravityVector().Y;
	if ( -100.0f< GravityForce)
	{
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 255, 255), MapLeftDownCheck - float4::LEFT);
		unsigned int DownCenterColor = GetGroundColor(RGB(255, 255, 255), MapDownCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 255, 255), MapRightDownCheck - float4::RIGHT);
		if (RGB(255, 255, 255) == LeftDownColor &&
			RGB(255, 255, 255) == DownCenterColor &&
			RGB(255, 255, 255) == RightDownColor)
		{
			int CurFrame = static_cast<int>(MainRenderer->GetCurFrame());
			SetAnimation("Tumbling", CurFrame);

			// "Tumbling" �ִϸ��̼��� ������ "Fall" ���·� ������Ʈ
			if (true == MainRenderer->IsAnimationEnd())
			{
				ChangeState(PlayerState::Fall);
			}
		}
		else
		{
			ChangeState(PlayerState::Fall);
		}

	}
	else
	{
		SetAnimation("Jump");
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