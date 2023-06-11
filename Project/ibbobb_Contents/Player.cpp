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
	CurDir = PlayerDir::UpRight;

	// 이건 무슨 함수?
	//MainRenderer->SetRenderScaleToTexture();
	
	Ratio = 2.0f;

	// 중력값 설정
	SetGravityPower(1000.0f);

	// 점프력 설정
	SetJumpForce(500.0f);

	// 속도 설정
	SetSpeed(200.0f);
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
	default:
		break;
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
		Data.Pos = ActorCameraPos() + MapUpCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapDownCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	}
}

void Player::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;

	switch (CurDir)
	{
	case PlayerDir::UpLeft:
		AnimationName = "UpLeft_";
		break;
	case PlayerDir::UpRight:
		AnimationName = "UpRight_";
		break;
	case PlayerDir::DownLeft:
		AnimationName = "DownLeft_";
		break;
	case PlayerDir::DownRight:
		AnimationName = "DownRight_";
		break;
	default:
		break;
	}

	AnimationName += _State;
	MainRenderer->ChangeAnimation(AnimationName, _StartFrame);
}


void Player::ChangeState(PlayerState _State)
{
	State = _State;
}
