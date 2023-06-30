#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <vector>
class PlayLevel1_1 : public GameEngineLevel
{
public:
	PlayLevel1_1();
	~PlayLevel1_1();

	PlayLevel1_1(const PlayLevel1_1& _Other) = delete;
	PlayLevel1_1(PlayLevel1_1&& _Other) noexcept = delete;
	PlayLevel1_1& operator=(const PlayLevel1_1& _Other) = delete;
	PlayLevel1_1& operator=(PlayLevel1_1&& _Other) noexcept = delete;

protected:
	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;


private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	float4 WindowScale = float4::ZERO;

	class BackGround* Back = nullptr;

	class ibb* ibbPlayer = nullptr;
	class obb* obbPlayer = nullptr;

	std::vector<class RoadMonster*> RoadMonsters;

	std::vector<class JumpingMonster*> JumpingMonsters;
	std::vector<class Warp*> Warps;

	class Map* EXMap = nullptr;

	float ZoomScale = 1.5f;

};

