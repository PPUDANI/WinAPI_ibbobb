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

	//ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_RightIdle.bmp"), 1, 1);
	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_LeftRun.bmp"), 10, 1);
	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_RightIdle.bmp"), 1, 1);
	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("obb_RightRun.bmp"), 10, 1);
	ResourceManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_Player.bmp"), 5, 17);


	
	MainRenderer = CreateRenderer(RenderOrder::Play);
	MainRenderer->SetScaleRatio(1);
	//MainRenderer->CreateAnimation("Left_Run", "obb_LeftRun.bmp", 0, 9, 0.1f, true);

	MainRenderer->CreateAnimation("Left_Run", "obb_RightIdle.bmp", 0, 0, 0.1f, true);

	//MainRenderer->CreateAnimation("Left Idle", "obb_LeftIdle.bmp", 0, 0, 0.1f, true);
	//MainRenderer->CreateAnimation("Right_Idle", "obb_RightIdle.bmp", 0, 0, 0.1f, true);

	MainRenderer->CreateAnimation("Right_Run", "obb_RightRun.bmp", 0, 9, 0.1f, true);

	MainRenderer->ChangeAnimation("Right_Run");
	MainRenderer->SetRenderScaleToTexture();

	ChangeState(PlayerState::Run);
}

void Player::Update(float _Delta)
{
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

std::string Player::SetAnimDir(const std::string _State)
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
	return AnimationName;
}
