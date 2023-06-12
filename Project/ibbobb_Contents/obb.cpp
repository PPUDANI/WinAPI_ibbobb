#include "obb.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "Player.h"
#include <windows.h>

obb::obb()
{
}

obb::~obb()
{
}

void obb::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\obb\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_obb.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_obb.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::obb);

	// Idle Animation
	MainRenderer->CreateAnimation("Left_Idle", "Left_obb.bmp", 0, 0, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Idle", "Right_obb.bmp", 0, 0, 10.0f, true);

	// Blink Animation
	MainRenderer->CreateAnimation("Left_Blink", "Left_obb.bmp", 0, 1, 0.07f, true);
	MainRenderer->CreateAnimation("Right_Blink", "Right_obb.bmp", 0, 1, 0.07f, true);

	// Run Animation
	MainRenderer->CreateAnimation("Left_Run", "Left_obb.bmp", 12, 21, 0.08f, true);
	MainRenderer->CreateAnimation("Right_Run", "Right_obb.bmp", 12, 21, 0.08f, true);

	// Jump Animation
	MainRenderer->CreateAnimation("Left_Jump", "Left_obb.bmp", 22, 22, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Jump", "Right_obb.bmp", 22, 22, 10.0f, true);

	// Tumbling Animation
	MainRenderer->CreateAnimation("Left_Tumbling", "Left_obb.bmp", 23, 28, 0.05f, true);
	MainRenderer->CreateAnimation("Right_Tumbling", "Right_obb.bmp", 23, 28, 0.05f, true);

	// Fall Animation
	MainRenderer->CreateAnimation("Left_Fall", "Left_obb.bmp", 29, 30, 0.07f, true);
	MainRenderer->CreateAnimation("Right_Fall", "Right_obb.bmp", 29, 30, 0.07f, true);

	// Dead Animation
	MainRenderer->CreateAnimation("Left_Dead", "Left_obb.bmp", 35, 41, 0.03f, true);
	MainRenderer->CreateAnimation("Right_Dead", "Right_obb.bmp", 35, 41, 0.03f, true);

	// Crouch Animation
	MainRenderer->CreateAnimation("Left_Crouch", "Left_obb.bmp", 11, 11, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Crouch", "Right_obb.bmp", 11, 11, 10.0f, true);

	MainRenderer->SetScaleRatio(Ratio);

	SetPos({300, 600});

	MoveRightKey = VK_RIGHT;
	MoveLeftKey = VK_LEFT;
	JumpKey = VK_UP;
	CrouchKey = VK_DOWN;
}

