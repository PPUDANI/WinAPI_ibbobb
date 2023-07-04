#pragma once
#include "SubLevel.h"
#include <GameEnginePlatform/GameEngineSound.h>
#include <vector>

class Lobby : public SubLevel
{
public:
	// Constructor Destructor
	Lobby();
	~Lobby();

	// Delete Function
	Lobby(const Lobby& _Other) = delete;
	Lobby(Lobby&& _Other) noexcept = delete;
	Lobby& operator=(const Lobby& _Other) = delete;
	Lobby& operator=(Lobby&& _Other) noexcept = delete;

	inline static void Level1Clear()
	{
		Level1ClearValue = true;
	}
	inline static void Level2Clear()
	{
		Level2ClearValue = true;
	}
	inline static void Level3Clear()
	{
		Level3ClearValue = true;
	}
	inline static void Level4Clear()
	{
		Level4ClearValue = true;
	}

protected:
	void LevelStart(GameEngineLevel* _PrevLevel)override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	static bool Level1ClearValue;
	static bool Level2ClearValue;
	static bool Level3ClearValue;
	static bool Level4ClearValue;

	// 레벨 입장 상태
	static bool EnterLevel1;
	static bool EnterLevel2;
	static bool EnterLevel3;
	static bool EnterLevel4;

	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void Level1Start(float _DeltaTime);
	void Level2Start(float _DeltaTime);
	void Level3Start(float _DeltaTime);
	void Level4Start(float _DeltaTime);

	void LevelPlayerInit();
	void CreateCharacter();
	void LobbySettingInit();

	const std::string ColName = "Lobby_Collision.bmp";

	

	// 레벨 오픈 상태
	bool Level2Open = false;
	bool Level3Open = false;
	bool Level4Open = false;

	bool IsCharacterCreated = false;

	std::vector<class LobbyMedal*> Medals;

	class Warp* LobbyWarp = nullptr;

	class Fade* LobbyStartFade = nullptr;
	Fade* LobbyEndFade = nullptr;
	bool EndFadeInit = false;

	class LobbyLevelText* Level1Text = nullptr;
	LobbyLevelText* Level2Text = nullptr;
	LobbyLevelText* Level3Text = nullptr;
	LobbyLevelText* Level4Text = nullptr;

	class DefaultImage* Level2Lock = nullptr;
	DefaultImage* Level3Lock = nullptr;
	DefaultImage* Level4Lock = nullptr;

	class LevelDoor* Level1Door = nullptr;
	LevelDoor* Level2Door = nullptr;
	LevelDoor* Level3Door = nullptr;
	LevelDoor* Level4Door = nullptr;

	class DoorStar* Level1Star = nullptr;
	DoorStar* Level2Star = nullptr;
	DoorStar* Level3Star = nullptr;
	DoorStar* Level4Star = nullptr;

	GameEngineSoundPlayer BGMPlayer;
	GameEngineSoundPlayer EffectPlayer;
};