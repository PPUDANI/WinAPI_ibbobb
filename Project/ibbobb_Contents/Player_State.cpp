#include "Player.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

void Player::IdleUpdate(float _DeltaTime)
{
	// Run ���� Ȯ��
	// Idle ���·� �ٲ� �� ����Ű�� ������ ���� �� Run���� �ٲ�� �ְ� �ϱ����� IsPress�� Ű üũ
	if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump ���� üũ
	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsDown('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	SetAnimation("Idle");
}


void Player::RunUpdate(float _DeltaTime)
{
	// ��, �� �̵�
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpLeft;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpRight;
	}
	else
	{
		ChangeState(PlayerState::Idle);
		return;
	}
	AddPos(MovePos);

	// �̵��� ����
	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsDown('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// ����, �ٴ� üũ
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::DOWN);
	if (RGB(255, 255, 255) == Color)
	{
		ChangeState(PlayerState::Fall);
	}

	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// ����, �ٴ� üũ
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		AddPos(float4::UP);
		GravityReset();
		ChangeState(PlayerState::Idle);
	}

	// ��, �� �̵�
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpLeft;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpRight;
	}
	AddPos(MovePos);

	SetAnimation("Fall");
}


void Player::JumpUpdate(float _DeltaTime)
{
	float4 PastPos = GetPos();
	PlayerDir PastDir = CurDir;

	//����, �ٴ� üũ
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::UP);
	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		AddPos(float4::UP);
		GravityReset();
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
		return;
	}

	// ��, �� �̵�
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpLeft;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		CurDir = PlayerDir::UpRight;
	}
	AddPos(MovePos);

	// ���� �� ���� �ӵ����� ������ �� "Tumbling" ���·� ������Ʈ
	if ((JumpForce / 2.0f) >= PastPos.Y - GetPos().Y)
	{
		int CurFrame = MainRenderer->GetCurFrame();
		SetAnimation("Tumbling", CurFrame);

		// "Tumbling" �ִϸ��̼��� ������ "Fall" ���·� ������Ʈ
		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(PlayerState::Fall);
		}
		return;
	}

	SetAnimation("Jump");
}
