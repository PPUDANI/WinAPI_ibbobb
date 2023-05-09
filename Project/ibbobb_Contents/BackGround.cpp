#include "BackGround.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourceManager.h>

#pragma comment(lib, "MSimg32.lib")

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
	GameEngineWindowTexture* BackBuffer = GameEngineWindow::MainWindow.GetBackBuffer();
	GameEngineWindowTexture* FindTexture = ResourceManager::GetInst().FindTexture(FileName);
}

void BackGround::Release()
{

}

void BackGround::Inst(const std::string& _FileName)
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

