#include "Player.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineWindow.h>

void Player::IdleUpdate(float _DeltaTime)
{
	// Run 상태 확인
	// Idle 상태로 바뀐 후 방향키를 누르고 있을 시 Run으로 바뀔수 있게 하기위해 IsPress로 키 체크
	if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// Jump 상태 체크
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
	// 좌, 우 이동
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

	// 이동중 점프
	if (true == GameEngineInput::IsDown('W') || true == GameEngineInput::IsDown('S'))
	{
		SetGravityVector(-CurGravity * JumpForce);
		ChangeState(PlayerState::Jump);
		return;
	}

	// 공중, 바닥 체크
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), DownCheck + float4::DOWN);
	if (RGB(255, 255, 255) == Color)
	{
		ChangeState(PlayerState::Fall);
	}

	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// 공중, 바닥 체크
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

	// 좌, 우 이동
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

	//공중, 바닥 체크
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

	// 점프 중 일정 속도보다 낮아질 시 "Tumbling" 상태로 업데이트
	if ((JumpForce / 2.0f) >= PastPos.Y - GetPos().Y)
	{
		int CurFrame = MainRenderer->GetCurFrame();
		SetAnimation("Tumbling", CurFrame);

		// "Tumbling" 애니메이션이 끝나면 "Fall" 상태로 업데이트
		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(PlayerState::Fall);
		}
		return;
	}

	SetAnimation("Jump");
}
