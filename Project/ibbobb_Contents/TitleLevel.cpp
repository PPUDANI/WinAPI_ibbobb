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

	// PressText 持失
	PressText = CreateActor<DefaultImage>(UpdateOrder::UI);
	PressText->Init("Press_Text.bmp", 0.05f);
	PressText->LevitationOn();
	PressText->SetPos({750.0f, 370.0f});

	// ibb
	TitleibbPlayer = CreateActor<Titleibb>(UpdateOrder::Player);
	TitleibbPlayer->SetPos({ 500.0f, 650.0f });

	// obb
	TitleobbPlayer = CreateActor<Titleobb>(UpdateOrder::Player);
	TitleobbPlayer->SetPos({ 1000.0f, 650.0f });

	SoundLoadManager::LoadSound("TitleBGM.mp3");

}

void TitleLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('P'))
	{
		PressText->BlinkOn();
		GameStartValue = true;;
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
		PressText->BlinkOff();
		PressText->Off();
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
	BGMPlayer.SetVolume(0.5f);
}

void TitleLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	TitleibbPlayer->OverOff();
	TitleibbPlayer = nullptr;

	TitleobbPlayer->OverOff();
	TitleobbPlayer = nullptr;

	PressText->OverOff();
	PressText = nullptr;

	TitleBack->OverOff();
	TitleBack = nullptr;

	TitleFade->OverOff();
	TitleFade = nullptr;
}