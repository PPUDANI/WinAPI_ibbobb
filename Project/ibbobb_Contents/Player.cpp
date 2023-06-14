#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"

#include <windows.h>

Player::Player()
{

}

Player::~Player()
{

}



void Player::Start()
{
	Init();

	ChangeState(PlayerState::Fall);

	// 첫 방향 설정
	CurDir = PlayerDir::Right;

	// 이건 무슨 함수?
	//MainRenderer->SetRenderScaleToTexture();
	
	Ratio = 2.0f;

	// 중력값 설정
	SetGravityPower(DefaultGravityPower);

	// 점프력 설정
	JumpForce = 500.0f;

	// 속도 설정
	Speed = 200.0f;

	// 워프 중력 기본값 설정
	HoleDefaultGravityValue = 300.0f;
}

void Player::Update(float _DeltaTime)
{
	// 줌인 줌아웃 
	if (true == GameEngineInput::IsPress('Z'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.005f);
	}

	if (true == GameEngineInput::IsPress('X'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.005f);
		float ZoomScale = GameEngineWindow::MainWindow.GetDoubleBufferingCopyScaleRatio();
		if (1.0f > ZoomScale)
		{
			GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
		}
	}

	if (true == GameEngineInput::IsDown('O'))
	{

		CheckPosOn = !CheckPosOn;
	}

	// 상태에 따른 Update
	switch (State)
	{
	case PlayerState::Idle:
		IdleUpdate(_DeltaTime);
		break;
	case PlayerState::Run:
		RunUpdate(_DeltaTime);
		break;
	case PlayerState::Jump:
		JumpUpdate(_DeltaTime);
		break;
	case PlayerState::Fall:
		FallUpdate(_DeltaTime);
		break;
	case PlayerState::Dead:
		DeadUpdate(_DeltaTime);
		break;
	case PlayerState::Crouch:
		CrouchUpdate(_DeltaTime);
	default:
		break;
	}

	std::vector<GameEngineCollision*> _Col;

	if (true == BodyCollision->Collision(CollisionOrder::MonsterBody,
		_Col,
		CollisionType::CirCle,
		CollisionType::CirCle))
	{
		ChangeState(PlayerState::Dead);
	}

	//임시 카메라 위치
	CameraFocus();
}

void Player::Render(float _DeltaTime)
{
	if (true == CheckPosOn)
	{
		CollisionData Data;
		HDC dc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
		Data.Scale = { 3,3 };
		Data.Pos = ActorCameraPos() + MapLeftUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapMiddleUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapMiddleDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftMiddleCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightMiddleCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	}
}

void Player::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;
	if (ReverseValue == false)
	{
		switch (CurDir)
		{
		case PlayerDir::Left:
			AnimationName = "Left_";
			break;
		case PlayerDir::Right:
			AnimationName = "Right_";
			break;
		default:
			break;
		}
	}
	else
	{
		switch (CurDir)
		{
		case PlayerDir::Left:
			AnimationName = "ReverseLeft_";
			break;
		case PlayerDir::Right:
			AnimationName = "ReverseRight_";
			break;
		default:
			break;
		}
	}

	AnimationName += _State;
	MainRenderer->ChangeAnimation(AnimationName, _StartFrame);
}


void Player::ChangeState(PlayerState _State)
{
	State = _State;
}


void Player::ReverseDir()
{
	ReverseValue = !ReverseValue;

}