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
	float RunSpeed = Speed * 3.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		CurDir = PlayerDir::UpLeft;

		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 255, 255), MapLeftCenterCheck);

		if (RGB(255, 255, 255) == LeftCenterColor)
		{
			MovePos = { -RunSpeed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightCenterColor = GetGroundColor(RGB(255, 255, 255), MapRightCenterCheck);

		if (RGB(255, 255, 255) == RightCenterColor)
		{
			MovePos = { RunSpeed * _DeltaTime, 0.0f };
		}
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
	unsigned int LeftDownColor = GetGroundColor(RGB(255, 255, 255), MapLeftDownCheck + float4::DOWN);
	unsigned int DownCneterColor = GetGroundColor(RGB(255, 255, 255), MapDownCenterCheck + float4::DOWN);
	unsigned int RightDownColor = GetGroundColor(RGB(255, 255, 255), MapRightDownCheck + float4::DOWN);

	if (RGB(255, 255, 255) == LeftDownColor &&
		RGB(255, 255, 255) == DownCneterColor &&
		RGB(255, 255, 255) == RightDownColor)
	{
		ChangeState(PlayerState::Fall);
		return;
	}
	else
	{
		unsigned int CheckColor = GetGroundColor(RGB(255, 255, 255), MapDownCenterCheck);

		while (CheckColor != RGB(255, 255, 255))
		{
			CheckColor = GetGroundColor(RGB(255, 255, 255), MapDownCenterCheck);
			AddPos(float4::UP);
		}

		GravityReset();
	}

	SetAnimation("Run");
}


void Player::FallUpdate(float _DeltaTime)
{
	// 공중, 바닥 체크
	unsigned int LeftDownColor = GetGroundColor(RGB(255, 255, 255), MapLeftDownCheck);
	unsigned int DownCneterColor = GetGroundColor(RGB(255, 255, 255), MapDownCenterCheck);
	unsigned int RightDownColor = GetGroundColor(RGB(255, 255, 255), MapRightDownCheck);

	if (RGB(255, 255, 255) == LeftDownColor &&
		RGB(255, 255, 255) == DownCneterColor &&
		RGB(255, 255, 255) == RightDownColor)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		AddPos(float4::UP);
		GravityReset();
		ChangeState(PlayerState::Idle);
		return;
	}

	// 좌, 우 이동
	float SideMoveSpeed = Speed * 3.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		CurDir = PlayerDir::UpLeft;
		
		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 255, 255), MapLeftCenterCheck);
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 255, 255), MapLeftDownCheck + float4::LEFT * 1);
		if (RGB(255, 255, 255) == LeftCenterColor &&
			RGB(255, 255, 255) == LeftDownColor)
		{
			MovePos = { -SideMoveSpeed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightCenterColor = GetGroundColor(RGB(255, 255, 255), MapRightCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 255, 255), MapRightDownCheck + float4::RIGHT * 1);
		if (RGB(255, 255, 255) == RightCenterColor &&
			RGB(255, 255, 255) == RightDownColor)
		{
			MovePos = { SideMoveSpeed * _DeltaTime, 0.0f };
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

	// 상단 충돌 체크
	unsigned int LeftUpColor = GetGroundColor(RGB(255, 255, 255), MapLeftUpCheck);
	unsigned int UpCenterColor = GetGroundColor(RGB(255, 255, 255), MapUpCenterCheck);
	unsigned int RightUpColor = GetGroundColor(RGB(255, 255, 255), MapRightUpCheck);

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

	// 좌, 우 이동
	float SideMoveSpeed = Speed * 3.0f;
	float4 MovePos = float4::ZERO;
	if (true == GameEngineInput::IsPress('A'))
	{
		CurDir = PlayerDir::UpLeft;

		unsigned int LeftCenterColor = GetGroundColor(RGB(255, 255, 255), MapLeftCenterCheck);
		unsigned int LeftDownColor = GetGroundColor(RGB(255, 255, 255), MapLeftDownCheck);
		if (RGB(255, 255, 255) == LeftCenterColor &&
			RGB(255, 255, 255) == LeftDownColor)
		{
			MovePos = { -SideMoveSpeed * _DeltaTime, 0.0f };
		}
	}
	else if (true == GameEngineInput::IsPress('D'))
	{
		CurDir = PlayerDir::UpRight;

		unsigned int RightCenterColor = GetGroundColor(RGB(255, 255, 255), MapRightCenterCheck);
		unsigned int RightDownColor = GetGroundColor(RGB(255, 255, 255), MapRightDownCheck);
		if (RGB(255, 255, 255) == RightCenterColor &&
			RGB(255, 255, 255) == RightDownColor)
		{
			MovePos = { SideMoveSpeed * _DeltaTime, 0.0f };
		}
	}

	AddPos(MovePos);

	// 점프 중 일정 속도보다 낮아질 시 "Tumbling" 상태로 업데이트
	if ((JumpForce / 2.0f) >= PastPos.Y - GetPos().Y)
	{
		int CurFrame = static_cast<int>(MainRenderer->GetCurFrame());
		SetAnimation("Tumbling", CurFrame);

		// "Tumbling" 애니메이션이 끝나면 "Fall" 상태로 업데이트
		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(PlayerState::Fall);
		}
	}
	else
	{
		SetAnimation("Jump");
	}
}
