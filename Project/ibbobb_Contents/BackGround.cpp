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

	GameEngineWindowTexture* Texture = nullptr;
	if (false == ResourcesManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Map\\" + _FileName);
		ResourcesManager::GetInst().TextureLoad(FilePath.GetStringPath());
	}
	Texture = ResourcesManager::GetInst().FindTexture(_FileName);

	float4 Scale = Texture->GetScale();
	Renderer->SetTexture(_FileName);
	Renderer->SetRenderScale(Scale);
	SetPos({ Scale.hX(), Scale.hY() });
}

