#include "Map.h"

#include <GameEngineCore/ResourceManager.h>
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
}

void Map::Update(float _Delta)
{
}

void Map::Render()
{

}

void Map::Release()
{

}

void Map::Init(const std::string& _FileName, float _Ratio)
{
	if (false == ResourceManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEnginePath FilePath;

		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Map\\" + _FileName);

		GameEngineWindowTexture* Texture = ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());

		float4 Scale = Texture->GetScale();
		Scale *= _Ratio;

		SetPos({ GetPos().iX() - Scale.hX(), GetPos().iY() - Scale.hY() });
		GameEngineRenderer* Render = CreateRenderer(_FileName, RenderOrder::Map);
		Render->SetRenderScale(Scale);
	}
}
