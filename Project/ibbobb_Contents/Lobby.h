#pragma once
#include "SubLevel.h"
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

protected:
	void LevelStart(GameEngineLevel* _PrevLevel)override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void Level1Start(float _DeltaTime);
	void Level2Start(float _DeltaTime);
	void Level3Start(float _DeltaTime);

	void LevelPlayerInit();
	void CreateCharacter();
	void LobbySettingInit();

	const std::string ColName = "Lobby_Collision.bmp";

	bool Level1On = false;
	bool Level2On = false;
	bool Level3On = false;
	bool EndFadeInit = false;

	bool IsCharacterCreated = false;

	std::vector<class LobbyMedal*> Medals;

	class Warp* LobbyWarp = nullptr;
	class Fade* LobbyStartFade = nullptr;
	Fade* LobbyEndFade = nullptr;
};