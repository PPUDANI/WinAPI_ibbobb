#include "TitleLevel.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineCore.h>

#include "ContentsEnum.h"
#include "SoundLoadManager.h"
#include "BackGround.h"
#include "DefaultImage.h"
#include "SubLevel.h"
#include "Fade.h"
#include "Titleibb.h"
#include "Titleobb.h"

TitleLevel::TitleLevel()
{

}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::Start()
{
	// BackGround 持失
	TitleBack = CreateActor<BackGround>(UpdateOrder::BackGround);
	TitleBack->Init("Title_BackGround.bmp");

	// Press'P' Text 持失
	PressText = CreateActor<DefaultImage>(UpdateOrder::UI);
	PressText->Init("Press_Text.bmp", 0.05f);
	PressText->LevitationOn();
	PressText->SetPos({750.0f, 370.0f});

	// Development Mode Text 持失
	DevelopmentModeText = CreateActor<DefaultImage>(UpdateOrder::UI);
	DevelopmentModeText->Init("DevelopmentMode_Texture.bmp", 0.05f);
	DevelopmentModeText->HideOn();
	DevelopmentModeText->SetPos({ 750.0f, 370.0f });

	// ibb
	TitleibbPlayer = CreateActor<Titleibb>(UpdateOrder::Player);
	TitleibbPlayer->SetPos({ 500.0f, 650.0f });

	// obb
	TitleobbPlayer = CreateActor<Titleobb>(UpdateOrder::Player);
	TitleobbPlayer->SetPos({ 1000.0f, 650.0f });

	// Sound
	SoundLoadManager::LoadSound("BGM", "TitleBGM.mp3");
	SoundLoadManager::LoadSound("LevelEffect", "PressKey.mp3");
}

void TitleLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('P') && false == GameStartValue)
	{
		EffectPlayer = GameEngineSound::SoundPlay("PressKey.mp3");
		EffectPlayer.SetVolume(0.4f);
		PressText->BlinkOn();
		GameStartValue = true;
	}
	else if (true == GameEngineInput::IsDown(VK_F12) && false == GameStartValue)
	{
		PressText->HideOn();
		EffectPlayer = GameEngineSound::SoundPlay("PressKey.mp3");
		EffectPlayer.SetVolume(0.4f);
		DevelopmentModeText->HideOff();
		DevelopmentModeText->BlinkOn();
		GameStartValue = true;
		SubLevel::DevelopmentModeOn();
	}

	if (true == GameStartValue)
	{
		LobbyStart(_DeltaTime);
	}
}

void TitleLevel::LobbyStart(float _DeltaTime)
{
	static float Time = 0.0f;
	static bool FadeOn = false;
	Time += _DeltaTime;

	if (Time >= 2.0f)
	{
		BGMPlayer.Stop();
		GameEngineCore::ChangeLevel("Lobby");
	}
	else if (Time >= 0.5f)
	{
		if (true ==SubLevel::IsDevelopmentMode())
		{
			DevelopmentModeText->BlinkOff();
			DevelopmentModeText->Off();
		}
		else
		{
			PressText->BlinkOff();
			PressText->Off();
		}
		
		if (false == FadeOn)
		{
			FadeOn = true;
			TitleFade = CreateActor<Fade>();
			TitleFade->Init("FadeBlack.bmp", FadeState::FadeOut);
			TitleFade->SetFadeSpeed(200.0f);
		}
		GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(Time + 0.5f);
	}
}

void TitleLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineSound::SoundPlay("TitleBGM.mp3");
	BGMPlayer.SetLoop(10);
	BGMPlayer.SetVolume(0.3f);
}

void TitleLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	TitleibbPlayer->OverOff();
	TitleibbPlayer = nullptr;

	TitleobbPlayer->OverOff();
	TitleobbPlayer = nullptr;

	PressText->OverOff();
	PressText = nullptr;

	DevelopmentModeText->OverOff();
	DevelopmentModeText = nullptr;

	TitleBack->OverOff();
	TitleBack = nullptr;

	TitleFade->OverOff();
	TitleFade = nullptr;
}