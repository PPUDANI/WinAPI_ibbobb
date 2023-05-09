#include "BackGround.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourceManager.h>

#pragma comment(lib, "msimg32.lib")

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

void BackGround::Start()
{
	SetPos({ 640, 360 });
}

void BackGround::Update(float _Delta)
{

}

void BackGround::Render()
{
	GameEngineWindowTexture* FindTexture = ResourceManager::GetInst().FindTexture(FileName);

	if (nullptr == FindTexture)
	{
		return;
	}

	GameEngineWindowTexture* BackBuffer = GameEngineWindow::MainWindow.GetBackBuffer();
	float4 Scale = FindTexture->GetScale();

	Scale *= 2.0f;

	BackBuffer->TransCopy(FindTexture, GetPos(), Scale, { 0, 0 }, FindTexture->GetScale());
}

void BackGround::Release()
{

}

void BackGround::Init(const std::string& _FileName)
{
	FileName = _FileName;

	if (false == ResourceManager::GetInst().IsLoadTexture(_FileName))
	{
		GameEnginePath FilePath;

		FilePath.GetCurrentPath();

		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\" + _FileName);

		GameEngineWindowTexture* Text = ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());

		float4 Scale = Text->GetScale();

		Scale.X *= 5.0f;
		Scale.Y *= 5.0f;

		SetScale(Scale);
	}
}

