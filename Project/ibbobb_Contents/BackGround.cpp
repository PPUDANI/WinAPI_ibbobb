#include "BackGround.h"
#include "ContentsEnum.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourceManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

BackGround::BackGround()
{
	
}


BackGround::~BackGround()
{

}

void BackGround::Start()
{
	SetPos({ 0, 0 });
}

void BackGround::Update(float _Delta)
{

}

void BackGround::Render(float _DeltaTime)
{

}

void BackGround::Release()
{

}

void BackGround::Init(const std::string& _FileName)
{

	if (false == ResourceManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEnginePath FilePath;

		FilePath.SetCurrentPath();

		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Map\\" + _FileName);

		GameEngineWindowTexture* Texture = ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());

		float4 Scale = Texture->GetScale();

		SetPos({ GetPos().iX() + Scale.hX(), GetPos().iY() + Scale.hY() });

		GameEngineRenderer* Render = CreateRenderer(_FileName, RenderOrder::BackGround);
		Render->SetRenderScale(Scale);
	}

}

