#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourceManager.h>
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

	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_Right.bmp"), 7, 6);
	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_Left.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::Play);
	MainRenderer->CreateAnimation("Left_Idle", "obb_Left.bmp", 0, 0, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Idle", "obb_Right.bmp", 0, 0, 10.0f, true);

	MainRenderer->CreateAnimation("Left_Run", "obb_Left.bmp", 13, 22, 0.1f, true);
	MainRenderer->CreateAnimation("Right_Run", "obb_Right.bmp", 13, 22, 0.1f, true);

	MainRenderer->ChangeAnimation("Left_Idle");

	MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(4.0f);

	ChangeState(PlayerState::Idle);
	Dir = PlayerDir::Right;
	SetPos({ 700, 400 });
}

void Player::Update(float _Delta)
{
	if (true == GameEngineInput::IsPress('I'))
	{
		float4 Scale;

		Scale.X = 1400.0f / 200.0f;
		Scale.Y = 800.0f / 200.0f;

		GameEngineWindow::ZoomScale += Scale;
	}

	if (true == GameEngineInput::IsPress('O'))
	{
		if (GameEngineWindow::ZoomScale.X > 0 && GameEngineWindow::ZoomScale.Y > 0)
		{
			float4 Scale;
			Scale.X = 1400.0f / 200.0f;
			Scale.Y = 800.0f / 200.0f;

			GameEngineWindow::ZoomScale -= Scale;
		}
		else
		{
			GameEngineWindow::ZoomScale = float4::ZERO;
		}
	}

	switch (State)
	{
	case PlayerState::Idle:
		return IdleUpdate(_Delta);
	case PlayerState::Run:
		return RunUpdate(_Delta);
	default:
		break;
	}

	

}

void Player::SetAnimDir(const std::string _State)
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
