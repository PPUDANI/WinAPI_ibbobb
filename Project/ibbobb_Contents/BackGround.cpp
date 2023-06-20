#include "BackGround.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

BackGround::BackGround()
{
	
}

BackGround::~BackGround()
{

}

void BackGround::Start()
{
	
}

void BackGround::Init(const std::string& _FileName)
{
	Renderer = CreateRenderer(RenderOrder::BackGround);

	GameEnginePath FilePath;

	if (false == ResourcesManager::GetInst().IsLoadTexture(_FileName))
	{
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Map\\" + _FileName);
	}

	GameEngineWindowTexture* Texture = ResourcesManager::GetInst().TextureLoad(FilePath.GetStringPath());
	
	float4 Scale = Texture->GetScale();
	Renderer->SetTexture(_FileName);
	Renderer->SetRenderScale(Scale);
	SetPos({ Scale.hX(), Scale.hY() });
}

