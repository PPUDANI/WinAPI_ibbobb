#include "PlayerIbb.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

#include "PlayerEnum.h"

void PlayerIbb::IdleUpdate(float _DeltaTime)
{
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::DOWN);

	if (RGB(255, 255, 255) == Color)
	{
		ChangeState(PlayerState::Fall);
	}

	if (true == GameEngineInput::IsPress(VK_UP) ||
		true == GameEngineInput::IsPress(VK_LEFT) ||
		true == GameEngineInput::IsPress(VK_DOWN) ||
		true == GameEngineInput::IsPress(VK_RIGHT))
	{
		ChangeState(PlayerState::Run);
		return;
	}
}

void PlayerIbb::RunUpdate(float _DeltaTime)
{
	SetAnimation("Run");

	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress(VK_LEFT))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress(VK_RIGHT))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Right;
	}
	else if (true == GameEngineInput::IsPress(VK_UP))
	{
		MovePos = { 0.0f, -Speed * _DeltaTime };
	}
	else if (true == GameEngineInput::IsPress(VK_DOWN))
	{
		MovePos = { 0.0f, Speed * _DeltaTime };
	}
	else
	{
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
	}

	AddPos(MovePos);
}

void PlayerIbb::FallUpdate(float _DeltaTime)
{
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::DOWN);

	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_DeltaTime);
		SetAnimation("Fall");
	}
	else
	{
		GravityReset();
		ChangeState(PlayerState::Idle);
		SetAnimation("Idle");
	}

	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress(VK_LEFT))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Left;
	}
	else if (true == GameEngineInput::IsPress(VK_RIGHT))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::Right;
	}
	AddPos(MovePos);
}

void PlayerIbb::ChangeState(PlayerState _State)
{
	State = _State;
}

