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
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_UpRight.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_UpLeft.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::Play);

	// UP 방향 애니메이션
	{
		// Idle Animation
		MainRenderer->CreateAnimation("UpLeft_Idle", "obb_UpLeft.bmp", 0, 0, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Idle", "obb_UpRight.bmp", 0, 0, 10.0f, true);

		// Blink Animation
		MainRenderer->CreateAnimation("UpLeft_Blink", "obb_UpLeft.bmp", 0, 1, 0.07f, true);
		MainRenderer->CreateAnimation("UpRight_Blink", "obb_UpRight.bmp", 0, 1, 0.07f, true);

		// Run Animation
		MainRenderer->CreateAnimation("UpLeft_Run", "obb_UpLeft.bmp", 13, 22, 0.08f, true);
		MainRenderer->CreateAnimation("UpRight_Run", "obb_UpRight.bmp", 13, 22, 0.08f, true);

		// Jump Animation
		MainRenderer->CreateAnimation("UpLeft_Jump", "obb_UpLeft.bmp", 23, 23, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Jump", "obb_UpRight.bmp", 23, 23, 10.0f, true);

		// Tumbling Animation
		MainRenderer->CreateAnimation("UpLeft_Tumbling", "obb_UpLeft.bmp", 24, 29, 0.05f, true);
		MainRenderer->CreateAnimation("UpRight_Tumbling", "obb_UpRight.bmp", 24, 29, 0.05f, true);

		// Fall Animation
		MainRenderer->CreateAnimation("UpLeft_Fall", "obb_UpLeft.bmp", 30, 31, 0.07f, true);
		MainRenderer->CreateAnimation("UpRight_Fall", "obb_UpRight.bmp", 30, 31, 0.07f, true);
	}

	// 첫 상태 설정
	ChangeState(PlayerState::Fall);

	// 첫 방향 설정
	CurDir = PlayerDir::UpRight;

	// 첫 애니메이션 설정
	MainRenderer->ChangeAnimation("UpLeft_Idle");
	MainRenderer->SetRenderScaleToTexture();

	Ratio = 1.0f;
	MainRenderer->SetScaleRatio(Ratio);

	// 중력값 설정
	SetGravityPower(3.0f);

	// 점프력 설정
	SetJumpForce(1.0f);

	// 속도 설정
	SetSpeed(100.0f);

	// 위치 설정
	SetPos({ 500.0f, 400.0f });
}

void Player::Update(float _DeltaTime)
{
	
	// 줌인 줌아웃 
	if (true == GameEngineInput::IsPress('I'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.005f);
	}

	if (true == GameEngineInput::IsPress('O'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.005f);
		float ZoomScale = GameEngineWindow::MainWindow.GetDoubleBufferingCopyScaleRatio();
		if (1.0f > ZoomScale)
		{
			GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
		}
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
	default:
		break;
	}

	//임시 카메라 위치
	CameraFocus();
}

void Player::Render(float _DeltaTime)
{
	//CollisionData Data;
	//HDC dc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
	//Data.Scale = { 5,5 };
	//Data.Pos = ActorCameraPos() + MapLeftUpCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	//Data.Pos = ActorCameraPos() + MapRightUpCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	//Data.Pos = ActorCameraPos() + MapUpCenterCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

	//Data.Pos = ActorCameraPos() + MapLeftDownCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	//Data.Pos = ActorCameraPos() + MapRightDownCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	//Data.Pos = ActorCameraPos() + MapDownCenterCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

	//Data.Pos = ActorCameraPos() + MapLeftCenterCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	//Data.Pos = ActorCameraPos() + MapRightCenterCheck;
	//Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
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
