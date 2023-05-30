#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

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
		// Idle
		MainRenderer->CreateAnimation("UpLeft_Idle", "obb_UpLeft.bmp", 0, 0, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Idle", "obb_UpRight.bmp", 0, 0, 10.0f, true);

		// Run
		MainRenderer->CreateAnimation("UpLeft_Run", "obb_UpLeft.bmp", 13, 22, 0.1f, true);
		MainRenderer->CreateAnimation("UpRight_Run", "obb_UpRight.bmp", 13, 22, 0.1f, true);

		// Jump
		MainRenderer->CreateAnimation("UpLeft_Jump", "obb_UpLeft.bmp", 23, 23, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Jump", "obb_UpRight.bmp", 23, 23, 10.0f, true);

		// Tumbling
		MainRenderer->CreateAnimation("UpLeft_Tumbling", "obb_UpLeft.bmp", 24, 29, 0.05f, true);
		MainRenderer->CreateAnimation("UpRight_Tumbling", "obb_UpRight.bmp", 24, 29, 0.05f, true);

		// Fall
		MainRenderer->CreateAnimation("UpLeft_Fall", "obb_UpLeft.bmp", 30, 31, 0.07f, true);
		MainRenderer->CreateAnimation("UpRight_Fall", "obb_UpRight.bmp", 30, 31, 0.07f, true);
	}

	MainRenderer->ChangeAnimation("UpLeft_Idle");
	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(2.0f);

	// 중력값
	SetGravityPower(2.5f);

	// 점프력 설정
	JumpForce = 1.5f;

	SetPos({ 200.0f, 200.0f });
	ChangeState(PlayerState::Fall);
	Dir = PlayerDir::UpRight;
}

void Player::Update(float _DeltaTime)
{
	
	// 줌인 줌아웃 
	if (true == GameEngineInput::IsPress('I'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.01f);
	}

	if (true == GameEngineInput::IsPress('O'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.01f);
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
	int a = MainRenderer->CurAnimation->CurFrame;
	CameraFocus();
}

void Player::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;

	switch (Dir)
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
