#pragma once
#include "SubLevel.h"

#include <GameEnginePlatform/GameEngineSound.h>
#include <vector>

class PlayLevel2 : public SubLevel
{
public:
	// Constructor Destructor
	PlayLevel2();
	~PlayLevel2();

	// Delete Function
	PlayLevel2(const PlayLevel2& _Other) = delete;
	PlayLevel2(PlayLevel2&& _Other) noexcept = delete;
	PlayLevel2& operator=(const PlayLevel2& _Other) = delete;
	PlayLevel2& operator=(PlayLevel2&& _Other) noexcept = delete;

protected:
	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;


private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void LevelPlayerInit();
	void SetZoomScale(float _Ratio, float _DeltaTime);

	void LobbyStart(float _DeltaTime);
	void Level2SettingInit();

	const std::string ColName = "Level2_Collision.bmp";


	std::vector<class RoadMonster*> RoadMonsters;
	std::vector<class JumpingMonster*> JumpingMonsters;
	std::vector<class Medal*> Medals;
	std::vector<class Warp*> Warps;


	RoadMonster* _RoadMonster = nullptr;
	JumpingMonster* _JumpingMonster = nullptr;
	Medal* _Medal = nullptr;
	Warp* _Warp = nullptr;
	class DefaultImage* GoalText = nullptr;


	class Fade* Level2StartFade = nullptr;
	Fade* Level2EndFade = nullptr;
	bool EnterLobby = false;
	bool EndFadeInit = false;

	class LevelDoor* LobyDoor = nullptr;

	GameEngineSoundPlayer BGMPlayer;
	GameEngineSoundPlayer EffectPlayer;

};