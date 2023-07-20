#include "Fade.h"

#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "ContentsEnum.h"

Fade::Fade()
{
}

Fade::~Fade()
{
}

void Fade::Init(const std::string& _FileName, FadeState _State)
{
	if (nullptr == ResourcesManager::GetInst().FindTexture(_FileName))
	{
		GameEnginePath Path;

		Path.SetCurrentPath();
		Path.MoveParentToExistsChild("Resources");
		Path.MoveChild("Resources\\Texture\\Map\\");

		ResourcesManager::GetInst().TextureLoad(Path.PlusFilePath(_FileName));
	}

	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	Renderer = CreateUIRenderer(RenderOrder::UI);
	Renderer->SetTexture(_FileName);
	Renderer->SetRenderPos(WinScale.Half());
	Renderer->SetRenderScale(WinScale);

	if (_State == FadeState::FadeIn)
	{
		AlphaValue = 255.0f;
	}
	State = _State;
}

void Fade::Start()
{
	
}

void Fade::Update(float _DeltaTime)
{

	if (State == FadeState::FadeIn)
	{
		AlphaValue -= FadeSpeed * _DeltaTime;
		if (0.0f >= AlphaValue)
		{
			FadeEnd = true;
			return;
		}
	}
	else if (State == FadeState::FadeOut)
	{
		AlphaValue += FadeSpeed * _DeltaTime;
		if (255.0f <= AlphaValue)
		{
			FadeEnd = true;
			return;
		}
	}

	Renderer->SetAlpha(static_cast<unsigned char>(AlphaValue));
}