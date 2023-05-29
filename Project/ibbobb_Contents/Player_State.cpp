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

	if (RGB(255, 255, 255) == Color)
	{
		ChangeState(PlayerState::Fall);
		SetAnimation("Fall");
	}

	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsPress('S'))
	{
		SetGravityVector(float4::UP * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
	{
		ChangeState(PlayerState::Run);
		return;
	}
}

void Player::RunUpdate(float _DeltaTime)
{
	SetAnimation("Run");

	// 왼쪽, 오른쪽 이동
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

	// 이동중 점프
	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsPress('S'))
	{
		SetGravityVector(float4::UP * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// 바닥 체크
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);
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

	float4 CurPos = GetPos();
	if (0.0f >= PastPos.Y - CurPos.Y)
	{
		ChangeState(PlayerState::Fall);
		
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