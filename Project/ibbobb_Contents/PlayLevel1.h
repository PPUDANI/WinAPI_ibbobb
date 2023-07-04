#pragma once
#include "SubLevel.h"
#include <GameEnginePlatform/GameEngineSound.h>
#include <vector>
class PlayLevel1 : public SubLevel
{
public:
	PlayLevel1();
	~PlayLevel1();

	PlayLevel1(const PlayLevel1& _Other) = delete;
	PlayLevel1(PlayLevel1&& _Other) noexcept = delete;
	PlayLevel1& operator=(const PlayLevel1& _Other) = delete;
	PlayLevel1& operator=(PlayLevel1&& _Other) noexcept = delete;

	

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
	void Level1SettingInit();

	const std::string ColName = "Level1_Collision.bmp";

	
	

	std::vector<class RoadMonster*> RoadMonsters;
	std::vector<class JumpingMonster*> JumpingMonsters;
	std::vector<class Medal*> Medals;
	std::vector<class Warp*> Warps;
	

	RoadMonster* _RoadMonster = nullptr;
	JumpingMonster* _JumpingMonster = nullptr;
	Medal* _Medal = nullptr;
	Warp* _Warp = nullptr;
	class DefaultImage* GoalText = nullptr;
	

	class Fade* Level1StartFade = nullptr;
	Fade* Level1EndFade = nullptr;
	bool EnterLobby = false;
	bool EndFadeInit = false;

	class LevelDoor* LobyDoor = nullptr;

	GameEngineSoundPlayer BGMPlayer;
	GameEngineSoundPlayer EffectPlayer;
	
};

