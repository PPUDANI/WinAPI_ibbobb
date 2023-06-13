#include "RoadMonster.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

RoadMonster::RoadMonster()
{

}

RoadMonster::~RoadMonster()
{

}



void RoadMonster::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Monster\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_RoadMonster.bmp"), 3, 4);
	Renderer = CreateRenderer(RenderOrder::RoadMonster);

	Renderer->CreateAnimation("Left_Idle", "Left_RoadMonster.bmp", 0, 0, 10.0f, true);
	Renderer->CreateAnimation("Left_Blink", "Left_RoadMonster.bmp", 0, 1, 0.2f, true);
	Renderer->CreateAnimation("Left_Dead", "Left_RoadMonster.bmp", 1, 11, 0.02f, true);
	Renderer->ChangeAnimation("Left_Idle");

	BodyCollision = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollision->SetCollisionScale({ 60, 60 });
	BodyCollision->SetCollisionType(CollisionType::CirCle);

	SetGravityPower(1000.0f);

	SetPos({ 600.0f, 600.0f });
}

void RoadMonster::Update(float _DeltaTime)
{
	unsigned int Color = GetGroundColor(RGB(255, 0, 0), float4::ZERO);

	if (RGB(255, 0, 0) != Color)
	{ 
		Gravity(_DeltaTime);
	}
	else
	{
		GravityReset();
	}
}
