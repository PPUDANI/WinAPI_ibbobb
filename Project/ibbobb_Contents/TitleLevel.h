#pragma once
#include <GameEngineCore/GameEngineLevel.h>

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

	class BackGround* Title;
	class BlinkImage* PressText;
	class Titleibb* TitleibbPlayer = nullptr;
	class Titleobb* TitleobbPlayer = nullptr;


};

