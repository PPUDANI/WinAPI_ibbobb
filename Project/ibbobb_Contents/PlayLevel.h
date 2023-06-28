#pragma once
#include <GameEngineCore/GameEngineLevel.h>

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
	void Release() override;

	float4 WindowScaleHalf = float4::ZERO;

	class BackGround* UpBack = nullptr;
	class BackGround* DownBack = nullptr;

	class ibb* ibbPlayer = nullptr;
	class obb* obbPlayer = nullptr;

	class RoadMonster* RoadMonster0 = nullptr;
	class RoadMonster* RoadMonster1 = nullptr;
	class JumpingMonster* JumpingMonster0 = nullptr;
	class Map* EXMap = nullptr;
};

