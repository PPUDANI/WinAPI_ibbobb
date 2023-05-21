#include "Player.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void Player::IdleUpdate(float _Delta)
{

	if (true == GameEngineInput::IsDown('W') ||
		true == GameEngineInput::IsDown('A') ||
		true == GameEngineInput::IsDown('S') ||
		true == GameEngineInput::IsDown('D') )
	{
		ChangeState(PlayerState::Run);
		return;
	}
}

void Player::RunUpdate(float _Delta)
{
	float Speed = 300.0f;
	SetAnimDir("Run");
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _Delta, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _Delta, 0.0f };
		Dir = PlayerDir::Right;
	}
	else if (true == GameEngineInput::IsPress('W'))
	{
		MovePos = { 0.0f, -Speed * _Delta };
	}
	else if (true == GameEngineInput::IsPress('S'))
	{
		MovePos = { 0.0f, Speed * _Delta };
	}
	else
	{
		SetAnimDir("Idle");
	}

	AddPos(MovePos);
	GetLevel()->GetMainCamera()->AddPos(MovePos);
}

void Player::ChangeState(PlayerState _State)
{
	State = _State;
}

