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

bool obb::IsLoadTexture = false;
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

	if (ResourcesManager::GetInst().FindSprite("Left_obb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_obb.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_obb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_obb.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Left_obb_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_obb_Reverse.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_obb_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_obb_Reverse.bmp"), 7, 6);
	}

	MainRenderer = CreateRenderer(RenderOrder::obb);
	float Frame;

	// Idle Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Idle", "Left_obb.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("Right_Idle", "Right_obb.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Idle", "Left_obb_Reverse.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Idle", "Right_obb_Reverse.bmp", 0, 0, Frame, true);

	// Blink Animation
	Frame = 0.07f;
	MainRenderer->CreateAnimation("Left_Blink", "Left_obb.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("Right_Blink", "Right_obb.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Blink", "Left_obb_Reverse.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Blink", "Right_obb_Reverse.bmp", 0, 1, Frame, true);

	// Crouch Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Crouch", "Left_obb.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("Right_Crouch", "Right_obb.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Crouch", "Left_obb_Reverse.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Crouch", "Right_obb_Reverse.bmp", 11, 11, Frame, true);

	// Run Animation
	Frame = 0.08f;
	MainRenderer->CreateAnimation("Left_Run", "Left_obb.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("Right_Run", "Right_obb.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Run", "Left_obb_Reverse.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Run", "Right_obb_Reverse.bmp", 12, 21, Frame, true);

	// Jump Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Jump", "Left_obb.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("Right_Jump", "Right_obb.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Jump", "Left_obb_Reverse.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Jump", "Right_obb_Reverse.bmp", 22, 22, Frame, true);

	// Tumbling Animation
	Frame = 0.05f;
	MainRenderer->CreateAnimation("Left_Tumbling", "Left_obb.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("Right_Tumbling", "Right_obb.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Tumbling", "Left_obb_Reverse.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Tumbling", "Right_obb_Reverse.bmp", 23, 28, Frame, true);

	// Fall Animation
	Frame = 0.07f;
	MainRenderer->CreateAnimation("Left_Fall", "Left_obb.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("Right_Fall", "Right_obb.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Fall", "Left_obb_Reverse.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Fall", "Right_obb_Reverse.bmp", 29, 30, Frame, true);

	// Dead Animation
	Frame = 0.03f;
	MainRenderer->CreateAnimation("Left_Dead", "Left_obb.bmp", 39, 41, Frame, true);
	MainRenderer->CreateAnimation("Right_Dead", "Right_obb.bmp", 39, 41, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Dead", "Left_obb_Reverse.bmp", 39, 41, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Dead", "Right_obb_Reverse.bmp", 39, 41, Frame, true);

	// 충돌체 설정
	BodyCollision = CreateCollision(CollisionOrder::obb);
	BodyCollision->SetCollisionScale({ 40, 40 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	MoveRightKey = VK_RIGHT;
	MoveLeftKey = VK_LEFT;
	JumpKey = VK_UP;
	CrouchKey = VK_DOWN;
}

