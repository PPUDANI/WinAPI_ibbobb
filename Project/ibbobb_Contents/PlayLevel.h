#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <vector>
class PlayLevel : public GameEngineLevel
{
public:
	PlayLevel();
	~PlayLevel();

	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) noexcept = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) noexcept = delete;

protected:
	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;


private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	float4 WindowScale = float4::ZERO;

	class BackGround* UpBack = nullptr;
	class BackGround* DownBack = nullptr;

	class ibb* ibbPlayer = nullptr;
	class obb* obbPlayer = nullptr;

	std::vector<class RoadMonster*> RoadMonsters;

	std::vector<class JumpingMonster*> JumpingMonsters;
	std::vector<class Warp*> Warps;

	class Map* EXMap = nullptr;

	float ZoomScale = 1.5f;

};

