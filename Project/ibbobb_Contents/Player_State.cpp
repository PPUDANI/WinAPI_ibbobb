#include "Player.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "PlayerEnum.h"

void Player::IdleUpdate(float _DeltaTime)
{
	SetAnimation("Idle");

	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);

	// Run ���� Ȯ��
	// Idle ���·� �ٲ� �� ����Ű�� ������ ���� �� Run���� �ٲ�� �ְ� �ϱ����� IsPress�� Ű üũ
	if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump ���� üũ
	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsPress('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}
}

void Player::RunUpdate(float _DeltaTime)
{
	SetAnimation("Run");

	// ����, ������ �̵�
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Right;
	}
	else
	{
		ChangeState(PlayerState::Idle);
	}

	// �̵��� ����
	if (true == GameEngineInput::IsPress('W') || true == GameEngineInput::IsPress('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// �ٴ� üũ
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::DOWN);
	if (RGB(255, 255, 255) == Color)
	{
		ChangeState(PlayerState::Fall);
	}

	AddPos(MovePos);
}

void Player::FallUpdate(float _DeltaTime)
{
	SetAnimation("Fall");
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

	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Right;
	}

	AddPos(MovePos);
}

void Player::JumpUpdate(float _DeltaTime)
{
	float4 PastPos = GetPos();

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

	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	float4 CurPos = GetPos();

	

	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Right;
	}
	AddPos(MovePos);

	if (1.05f >= PastPos.Y - CurPos.Y)
	{
		SetAnimation("Tumbling");

		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(PlayerState::Fall);
			return;
		}
		
	}
	else
	{
		SetAnimation("Jump");
	}
}

void Player::ChangeState(PlayerState _State)
{
	State = _State;
}