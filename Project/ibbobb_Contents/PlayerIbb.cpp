#include "PlayerIbb.h"

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

PlayerIbb::PlayerIbb()
{

}

PlayerIbb::~PlayerIbb()
{

}

void PlayerIbb::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("ibb_Right.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("ibb_Left.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::Play);

	// Idle
	MainRenderer->CreateAnimation("Left_Idle", "ibb_Left.bmp", 0, 0, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Idle", "ibb_Right.bmp", 0, 0, 10.0f, true);
	// Run
	MainRenderer->CreateAnimation("Left_Run", "ibb_Left.bmp", 13, 22, 0.09f, true);
	MainRenderer->CreateAnimation("Right_Run", "ibb_Right.bmp", 13, 22, 0.09f, true);

	// Fall
	MainRenderer->CreateAnimation("Left_Fall", "ibb_Left.bmp", 30, 31, 0.05f, true);
	MainRenderer->CreateAnimation("Right_Fall", "ibb_Right.bmp", 30, 31, 0.05f, true);

	MainRenderer->ChangeAnimation("Left_Idle");
	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(2.0f);
	SetGravityPower(2.5f);
	SetPos({ 300.0f, 200.0f });
	ChangeState(PlayerState::Idle);
	Dir = PlayerDir::Right;
}

void PlayerIbb::Update(float _DeltaTime)
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
		//RunUpdate(_DeltaTime);
		break;
	case PlayerState::Fall:
		FallUpdate(_DeltaTime);
		break;
	default:
		break;
	}

	CameraFocus();
}

void PlayerIbb::SetAnimation(const std::string _State)
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
