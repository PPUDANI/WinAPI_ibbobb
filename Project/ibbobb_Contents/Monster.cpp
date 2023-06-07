#include "Monster.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/ResourcesManager.h>
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>
Monster::Monster()
{

}

Monster::~Monster()
{

}

void Monster::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Monster_Sheet.bmp"), 3, 4);
	Renderer = CreateRenderer(RenderOrder::Monster);

	Renderer->CreateAnimation("Left_Idle", "Monster_Sheet.bmp", 0, 0, 10.0f, true);

	Renderer->CreateAnimation("Left_Blink", "Monster_Sheet.bmp", 0, 1, 0.2f, true);

	Renderer->CreateAnimation("Left_Dead", "Monster_Sheet.bmp", 1, 11, 0.03f, true);
	Renderer->ChangeAnimation("Left_Idle");
	SetPos({400, 700});
}

void Monster::Update(float _DeltaTime)
{
	unsigned int Color = GetGroundColor(RGB(255, 255, 255), float4::ZERO);

	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_DeltaTime);
	}
	else
	{
		Renderer->ChangeAnimation("Left_Dead");
		if (true == Renderer->IsAnimationEnd())
		{
			Death();
		}
	}
}