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

ibb* ibb::Mainibb = nullptr;

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

	if (ResourcesManager::GetInst().FindSprite("Left_ibb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_ibb.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_ibb.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_ibb.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Left_ibb_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_ibb_Reverse.bmp"), 7, 6);
	}

	if (ResourcesManager::GetInst().FindSprite("Right_ibb_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_ibb_Reverse.bmp"), 7, 6);
	}

	MainRenderer = CreateRenderer(RenderOrder::ibb);
	float Frame;

	// Idle Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Idle", "Left_ibb.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("Right_Idle", "Right_ibb.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Idle", "Left_ibb_Reverse.bmp", 0, 0, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Idle", "Right_ibb_Reverse.bmp", 0, 0, Frame, true);

	// Blink Animation
	Frame = 0.07f;
	MainRenderer->CreateAnimation("Left_Blink", "Left_ibb.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("Right_Blink", "Right_ibb.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Blink", "Left_ibb_Reverse.bmp", 0, 1, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Blink", "Right_ibb_Reverse.bmp", 0, 1, Frame, true);

	// Crouch Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Crouch", "Left_ibb.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("Right_Crouch", "Right_ibb.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Crouch", "Left_ibb_Reverse.bmp", 11, 11, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Crouch", "Right_ibb_Reverse.bmp", 11, 11, Frame, true);

	// Run Animation
	Frame = 0.08f;
	MainRenderer->CreateAnimation("Left_Run", "Left_ibb.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("Right_Run", "Right_ibb.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Run", "Left_ibb_Reverse.bmp", 12, 21, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Run", "Right_ibb_Reverse.bmp", 12, 21, Frame, true);

	// Jump Animation
	Frame = 10.0f;
	MainRenderer->CreateAnimation("Left_Jump", "Left_ibb.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("Right_Jump", "Right_ibb.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Jump", "Left_ibb_Reverse.bmp", 22, 22, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Jump", "Right_ibb_Reverse.bmp", 22, 22, Frame, true);

	// Tumbling Animation
	Frame = 0.05f;
	MainRenderer->CreateAnimation("Left_Tumbling", "Left_ibb.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("Right_Tumbling", "Right_ibb.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Tumbling", "Left_ibb_Reverse.bmp", 23, 28, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Tumbling", "Right_ibb_Reverse.bmp", 23, 28, Frame, true);

	// Fall Animation
	Frame = 0.07f;
	MainRenderer->CreateAnimation("Left_Fall", "Left_ibb.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("Right_Fall", "Right_ibb.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("ReverseLeft_Fall", "Left_ibb_Reverse.bmp", 29, 30, Frame, true);
	MainRenderer->CreateAnimation("ReverseRight_Fall", "Right_ibb_Reverse.bmp", 29, 30, Frame, true);

	// Dead Animation
	Frame = 0.03f;
	MainRenderer->CreateAnimation("Left_Dead", "Left_ibb.bmp", 39, 41, 0.04f, true);
	MainRenderer->CreateAnimation("Right_Dead", "Right_ibb.bmp", 39, 41, 0.04f, true);
	MainRenderer->CreateAnimation("ReverseLeft_Dead", "Left_ibb_Reverse.bmp", 39, 41, 0.04f, true);
	MainRenderer->CreateAnimation("ReverseRight_Dead", "Right_ibb_Reverse.bmp", 39, 41, 0.04f, true);

	// 충돌체 설정

	BodyCol = CreateCollision(CollisionOrder::ibbBody);
	BodyCol->SetCollisionScale({ 34.0f, 34.0f });
	BodyCol->SetCollisionType(CollisionType::Rect);

	LeftCol = CreateCollision(CollisionOrder::ibbLeft);
	LeftCol->SetCollisionScale({ 10.0f, 30.0f });
	LeftCol->SetCollisionPos({ -14.0f, 0.0f });
	LeftCol->SetCollisionType(CollisionType::Rect);

	RightCol = CreateCollision(CollisionOrder::ibbRight);
	RightCol->SetCollisionScale({ 10.0f, 30.0f });
	RightCol->SetCollisionPos({ 14.0f, 0.0f });
	RightCol->SetCollisionType(CollisionType::Rect);

	UpCol = CreateCollision(CollisionOrder::ibbUp);
	UpCol->SetCollisionScale({ 34.0f, 10.0f });
	UpCol->SetCollisionPos({ 0.0f, -12.0f });
	UpCol->SetCollisionType(CollisionType::Rect);

	DownCol = CreateCollision(CollisionOrder::ibbDown);
	DownCol->SetCollisionScale({ 34.0f, 10.0f });
	DownCol->SetCollisionPos({ 0.0f, 12.0f });
	DownCol->SetCollisionType(CollisionType::Rect);


	OtherPlayerBodyCol = static_cast<int>(CollisionOrder::obbBody);
	OtherPlayerLeftCol = static_cast<int>(CollisionOrder::obbLeft);
	OtherPlayerRightCol = static_cast<int>(CollisionOrder::obbRight);
	OtherPlayerUpCol = static_cast<int>(CollisionOrder::obbUp);
	OtherPlayerDownCol = static_cast<int>(CollisionOrder::obbDown);

	// 조작키
	MoveRightKey = 'D';
	MoveLeftKey = 'A';
	JumpKey = 'W';
	CrouchKey = 'S';
}

bool ibb::LeftToOtherRightCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return LeftCol->Collision(OtherPlayerRightCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}

bool ibb::RightToOtherLeftCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return RightCol->Collision(OtherPlayerLeftCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}

bool ibb::UpToOtherDownCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return UpCol->Collision(OtherPlayerDownCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}
bool ibb::DownToOtherUpCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return DownCol->Collision(OtherPlayerUpCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}

bool ibb::UpToOtherBodyCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return UpCol->Collision(OtherPlayerBodyCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}

bool ibb::DownToOtherBodyCheck()
{
	std::vector<GameEngineCollision*> _ColVec;
	return DownCol->Collision(OtherPlayerBodyCol,
		_ColVec,
		CollisionType::Rect,
		CollisionType::Rect);
}

void ibb::LevelStart()
{
	Mainibb = this;
	Mainibb->SetPos({ 100, 500 });
}
