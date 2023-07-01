#include "Medal.h"

#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <cmath>
#include "ContentsEnum.h"

std::vector<Medal*> Medal::MedalsByLevelIsAcquired;
Medal::Medal()
{
	
}

Medal::~Medal()
{
}

void Medal::Init()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Medal\\");

	if (ResourcesManager::GetInst().FindSprite("LevelMedal.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("LevelMedal.bmp"), 7, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Medal);

	// Idle
	Renderer->CreateAnimation("Idle", "LevelMedal.bmp", 0, 8, 0.15f, true);

	// Get
	Renderer->CreateAnimation("Get", "LevelMedal.bmp", 9, 12, 0.2f, true);

	CurState = MedalState::Idle;
	ReverseValue = false;

	MedalsByLevelIsAcquired.push_back(this);
}

void Medal::ReverseInit()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Medal\\");

	if (ResourcesManager::GetInst().FindSprite("LevelMedal_Reverse.bmp") == nullptr)
	{
		ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("LevelMedal_Reverse.bmp"), 7, 2);
	}

	Renderer = CreateRenderer(RenderOrder::Medal);

	// Idle
	Renderer->CreateAnimation("Idle", "LevelMedal_Reverse.bmp", 0, 8, 0.1f, true);

	// Get
	Renderer->CreateAnimation("Get", "LevelMedal_Reverse.bmp", 9, 12, 0.2f, true);

	CurState = MedalState::Idle;
	ReverseValue = true;

	MedalsByLevelIsAcquired.push_back(this);
}

void Medal::Start()
{

}

void Medal::Update(float _DeltaTime)
{
	switch (CurState)
	{
	case MedalState::Idle:
		IdleUpdate(_DeltaTime);
		break;
	case MedalState::Get:
		GetUpdate(_DeltaTime);
		break;
	case MedalState::Acquired:
		AcquiredUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Medal::LevelStart()
{

}

void Medal::IdleUpdate(float _DeltaTime)
{
	Radian += Speed;

	if (Radian == 180.0f)
	{
		Radian = 0.0f;
	}

	float yOffset = sinf(Radian) * MovingHeight;

	float4 add = { 0.0f, yOffset };
	AddPos(add * _DeltaTime);
	SetAnimation("Idle");
}

void Medal::GetUpdate(float _DeltaTime)
{

}

void Medal::AcquiredUpdate(float _DeltaTime)
{

}

void Medal::MedalArrayInit()
{

}
void Medal::SetAnimation(const std::string _Name)
{
	Renderer->ChangeAnimation(_Name);
}