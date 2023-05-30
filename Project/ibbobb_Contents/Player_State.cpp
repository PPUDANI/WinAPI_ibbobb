#include "Player.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

void Player::ChangeState(PlayerState _State)
{
	State = _State;
}


void Player::IdleUpdate(float _DeltaTime)
{
	SetAnimation("Idle");

	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck);

	// Run 상태 확인
	// Idle 상태로 바뀐 후 방향키를 누르고 있을 시 Run으로 바뀔수 있게 하기위해 IsPress로 키 체크
	if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump 상태 체크
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

	// 왼쪽, 오른쪽 이동
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::UpLeft;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::UpRight;
	}
	else
	{
		ChangeState(PlayerState::Idle);
	}

	// 이동중 점프
	if (true == GameEngineInput::IsPress('W') || true == GameEngineInput::IsPress('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// 바닥 체크
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
		Dir = PlayerDir::UpLeft;
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _DeltaTime, 0.0f };
		Dir = PlayerDir::UpRight;
	}

	AddPos(MovePos);
}


void Player::JumpUpdate(float _DeltaTime)
{
	float4 PastPos = GetPos();
	PlayerDir PastDir = Dir;
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

	// 좌, 우 이동
	float Speed = 300.0f;
	float4 MovePos = float4::ZERO;
	
	if (1.1f >= PastPos.Y - GetPos().Y) 
	{
		// 점프 중 일정 속도보다 낮아질 시 Tumbling으로 상태 업데이트
		//ChangeState(PlayerState::Tumbling);

		if (true == GameEngineInput::IsPress('A'))
		{
			MovePos = { -Speed * _DeltaTime, 0.0f };
			Dir = PlayerDir::UpLeft;
		}
		else if (true == GameEngineInput::IsPress('D'))
		{
			MovePos = { Speed * _DeltaTime, 0.0f };
			Dir = PlayerDir::UpRight;
		}

		int CurFrame = MainRenderer->GetCurFrame();
		SetAnimation("Tumbling", CurFrame);

		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(PlayerState::Fall);
		}

	}
	else
	{
		if (true == GameEngineInput::IsPress('A'))
		{
			MovePos = { -Speed * _DeltaTime, 0.0f };
			Dir = PlayerDir::UpLeft;
		}
		else if (true == GameEngineInput::IsPress('D'))
		{
			MovePos = { Speed * _DeltaTime, 0.0f };
			Dir = PlayerDir::UpRight;
		}

		SetAnimation("Jump");
	}

	AddPos(MovePos);
}
