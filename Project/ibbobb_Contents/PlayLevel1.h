#pragma once
#include "SubLevel.h"
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
	void SetZoomScale(float _Ratio, float _DeltaTime);
	std::vector<class RoadMonster*> RoadMonsters;
	std::vector<class JumpingMonster*> JumpingMonsters;
	std::vector<class Medal*> Medals;
	std::vector<class Warp*> Warps;

	RoadMonster* _RoadMonster = nullptr;
	JumpingMonster* _JumpingMonster = nullptr;
	Medal* _Medal = nullptr;
	Warp* _Warp = nullptr;
};

