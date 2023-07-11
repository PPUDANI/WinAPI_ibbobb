#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineSound.h>

class TitleLevel : public GameEngineLevel
{
public:
	TitleLevel();
	~TitleLevel();

	TitleLevel(const TitleLevel& _Other) = delete;
	TitleLevel(TitleLevel&& _Other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _Other) = delete;
	TitleLevel& operator=(TitleLevel&& _Other) noexcept = delete;

protected:
	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

private:

	void Start() override;
	void Update(float _DeltaTime) override;
	void LobbyStart(float _DeltaTime);

	bool GameStartValue = false;
	class BackGround* TitleBack;
	class DefaultImage* PressText;
	class DefaultImage* DevelopmentModeText;
	class Titleibb* TitleibbPlayer = nullptr;
	class Titleobb* TitleobbPlayer = nullptr;
	class Fade* TitleFade = nullptr;

	GameEngineSoundPlayer BGMPlayer;
	GameEngineSoundPlayer EffectPlayer;
};

