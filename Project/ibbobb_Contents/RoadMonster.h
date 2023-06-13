#pragma once
#include "GravityActor.h"

enum class RoadMonsterDir
{
	Left,
	Right,
	ReverseLeft,
	ReverseRight
};

class RoadMonster : public GravityActor
{
public:
	RoadMonster();
	~RoadMonster();

	RoadMonster(const RoadMonster& _Other) = delete;
	RoadMonster(RoadMonster&& _Other) noexcept = delete;
	RoadMonster& operator=(const RoadMonster& _Other) = delete;
	RoadMonster& operator=(RoadMonster&& _Other) noexcept = delete;

	inline void SetMovementDistance(float _MovementDistance)
	{
		MovementDistance = _MovementDistance;
	}

protected:
	
private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void SetAnimation(const std::string _State, int _StartFrame = 0);
	float Speed = 1.0f;
	float MovementDistance = 100.0f;
	RoadMonsterDir CurDir = RoadMonsterDir::Left;
	
	class GameEngineCollision* BodyCollision;
	class GameEngineRenderer* Renderer;
};

