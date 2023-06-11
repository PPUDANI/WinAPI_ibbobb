#include "ibb.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"

#include "Player.h"
#include <windows.h>

ibb::ibb()
{
}

ibb::~ibb()
{
}

void ibb::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\ibb\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_ibb.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_ibb.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::ibb);

	// Idle Animation
	MainRenderer->CreateAnimation("UpLeft_Idle", "Left_ibb.bmp", 0, 0, 10.0f, true);
	MainRenderer->CreateAnimation("UpRight_Idle", "Right_ibb.bmp", 0, 0, 10.0f, true);

	// Blink Animation
	MainRenderer->CreateAnimation("UpLeft_Blink", "Left_ibb.bmp", 0, 1, 0.07f, true);
	MainRenderer->CreateAnimation("UpRight_Blink", "Right_ibb.bmp", 0, 1, 0.07f, true);

	// Run Animation
	MainRenderer->CreateAnimation("UpLeft_Run", "Left_ibb.bmp", 12, 21, 0.08f, true);
	MainRenderer->CreateAnimation("UpRight_Run", "Right_ibb.bmp", 12, 21, 0.08f, true);

	// Jump Animation
	MainRenderer->CreateAnimation("UpLeft_Jump", "Left_ibb.bmp", 22, 22, 10.0f, true);
	MainRenderer->CreateAnimation("UpRight_Jump", "Right_ibb.bmp", 22, 22, 10.0f, true);

	// Tumbling Animation
	MainRenderer->CreateAnimation("UpLeft_Tumbling", "Left_ibb.bmp", 23, 28, 0.05f, true);
	MainRenderer->CreateAnimation("UpRight_Tumbling", "Right_ibb.bmp", 23, 28, 0.05f, true);

	// Fall Animation
	MainRenderer->CreateAnimation("UpLeft_Fall", "Left_ibb.bmp", 29, 30, 0.07f, true);
	MainRenderer->CreateAnimation("UpRight_Fall", "Right_ibb.bmp", 29, 30, 0.07f, true);

	// Dead Animation
	MainRenderer->CreateAnimation("UpLeft_Dead", "Left_ibb.bmp", 35, 41, 0.03f, true);
	MainRenderer->CreateAnimation("UpRight_Dead", "Right_ibb.bmp", 35, 41, 0.03f, true);

	// Crouch Animation
	MainRenderer->CreateAnimation("UpLeft_Crouch", "Left_ibb.bmp", 11, 11, 10.0f, true);
	MainRenderer->CreateAnimation("UpRight_Crouch", "Right_ibb.bmp", 11, 11, 10.0f, true);

	MainRenderer->SetScaleRatio(Ratio);
	SetPos({ 500, 600 });

    MoveRightKey = 'D';
	MoveLeftKey = 'A';
	JumpKey = 'W';
	CrouchKey = 'S';
}


//void ibb::Start()
//{
//	// 위치 설정
//	SetPos({ 600.0f, 900.0f });
//}
//
//void ibb::Update(float _DeltaTime)
//{
//}
