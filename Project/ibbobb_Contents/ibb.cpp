#include "ibb.h"

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
	MainRenderer->CreateAnimation("Left_Idle", "Left_ibb.bmp", 0, 0, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Idle", "Right_ibb.bmp", 0, 0, 10.0f, true);

	// Blink Animation
	MainRenderer->CreateAnimation("Left_Blink", "Left_ibb.bmp", 0, 1, 0.07f, true);
	MainRenderer->CreateAnimation("Right_Blink", "Right_ibb.bmp", 0, 1, 0.07f, true);

	// Crouch Animation
	MainRenderer->CreateAnimation("Left_Crouch", "Left_ibb.bmp", 11, 11, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Crouch", "Right_ibb.bmp", 11, 11, 10.0f, true);

	// Run Animation
	MainRenderer->CreateAnimation("Left_Run", "Left_ibb.bmp", 12, 21, 0.08f, true);
	MainRenderer->CreateAnimation("Right_Run", "Right_ibb.bmp", 12, 21, 0.08f, true);

	// Jump Animation
	MainRenderer->CreateAnimation("Left_Jump", "Left_ibb.bmp", 22, 22, 10.0f, true);
	MainRenderer->CreateAnimation("Right_Jump", "Right_ibb.bmp", 22, 22, 10.0f, true);

	// Tumbling Animation
	MainRenderer->CreateAnimation("Left_Tumbling", "Left_ibb.bmp", 23, 28, 0.05f, true);
	MainRenderer->CreateAnimation("Right_Tumbling", "Right_ibb.bmp", 23, 28, 0.05f, true);

	// Fall Animation
	MainRenderer->CreateAnimation("Left_Fall", "Left_ibb.bmp", 29, 30, 0.07f, true);
	MainRenderer->CreateAnimation("Right_Fall", "Right_ibb.bmp", 29, 30, 0.07f, true);

	// Dead Animation
	MainRenderer->CreateAnimation("Left_Dead", "Left_ibb.bmp", 39, 41, 0.04f, true);
	MainRenderer->CreateAnimation("Right_Dead", "Right_ibb.bmp", 39, 41, 0.04f, true);


	MainRenderer->SetScaleRatio(Ratio);
	SetPos({ 400, 600 });

	// 충돌체 설정
	BodyCollision = CreateCollision(CollisionOrder::PlayerBody);
	BodyCollision->SetCollisionScale({ 40, 40 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

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
