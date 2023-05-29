#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

#include "ContentsEnum.h"
#include "PlayerEnum.h"

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

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_Right.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_Left.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::Play);

	{
		// Idle
		MainRenderer->CreateAnimation("Left_Idle", "obb_Left.bmp", 0, 0, 10.0f, true);
		MainRenderer->CreateAnimation("Right_Idle", "obb_Right.bmp", 0, 0, 10.0f, true);

		// Run
		MainRenderer->CreateAnimation("Left_Run", "obb_Left.bmp", 13, 22, 0.1f, true);
		MainRenderer->CreateAnimation("Right_Run", "obb_Right.bmp", 13, 22, 0.1f, true);

		// Jump
		MainRenderer->CreateAnimation("Left_Jump", "obb_Left.bmp", 23, 23, 10.0f, true);
		MainRenderer->CreateAnimation("Right_Jump", "obb_Right.bmp", 23, 23, 10.0f, true);

		// Tumbling
		MainRenderer->CreateAnimation("Left_Tumbling", "obb_Left.bmp", 24, 29, 0.04f, true);
		MainRenderer->CreateAnimation("Right_Tumbling", "obb_Right.bmp", 24, 29, 0.04f, true);

		// Fall
		MainRenderer->CreateAnimation("Left_Fall", "obb_Left.bmp", 30, 31, 0.07f, true);
		MainRenderer->CreateAnimation("Right_Fall", "obb_Right.bmp", 30, 31, 0.07f, true);

		//
	}

	MainRenderer->ChangeAnimation("Left_Idle");
	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(2.0f);

	// 중력값
	SetGravityPower(2.5f);

	// 점프력 설정
	JumpForce = 1.5f;

	SetPos({ 200.0f, 200.0f });
	ChangeState(PlayerState::Fall);
	Dir = PlayerDir::Right;
}

void Player::Update(float _DeltaTime)
{
	
	
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

	CameraFocus();
}

void Player::SetAnimation(const std::string _State)
{
	std::string AnimationName;

	switch (Dir)
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

	AnimationName += _State;

	MainRenderer->ChangeAnimation(AnimationName);
}
