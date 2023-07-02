#include "Map.h"

#include <GameEngineCore/ResourcesManager.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

#include "ContentsEnum.h"

Map::Map()
{

}

Map::~Map()
{

}

void Map::Start()
{
	Renderer = CreateRenderer(RenderOrder::Map);
	DebugRenderer = CreateRenderer(RenderOrder::Map);

	Renderer->On();
	DebugRenderer->Off();
}

void Map::Update(float _Delta)
{
}

void Map::Render(float _DeltaTime)
{

}

void Map::Release()
{

}

void Map::Init(const std::string& _FileName, const std::string& _DebugFileName)
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Map\\");

	GameEngineWindowTexture* Texture = nullptr;

	if (false == ResourcesManager::GetInst().IsLoadTexture(_FileName))
	{
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath(_FileName));
	}

	Texture = ResourcesManager::GetInst().FindTexture(_FileName);

	float4 Scale = Texture->GetScale();
	Renderer->SetTexture(_FileName);
	Renderer->SetRenderScale(Scale);

	if (false == ResourcesManager::GetInst().IsLoadTexture(_DebugFileName))
	{
		Texture = ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath(_DebugFileName));
	}

	DebugRenderer->SetTexture(_DebugFileName);
	DebugRenderer->SetRenderScale(Scale);

	SetPos({ Scale.hX(), Scale.hY() });
}

void Map::SwitchRender()
{
	SwitchRenderValue = !SwitchRenderValue;

	if (SwitchRenderValue)
	{
		Renderer->On();
		DebugRenderer->Off();
	}
	else
	{
		Renderer->Off();
		DebugRenderer->On();
	}
}