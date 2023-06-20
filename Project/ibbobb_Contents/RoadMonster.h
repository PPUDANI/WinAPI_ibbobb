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

	inline void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	inline void SetDir(RoadMonsterDir _Dir)
	{
		CurDir = _Dir;
	}

	inline void ReverseDir()
	{
		ReverseValue = !ReverseValue;
	}

protected:
	
private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void SetAnimation(const std::string _State, int _StartFrame = 0);

	float Speed = 0.1f;
	float CurDistance = 0.0f;
	float MovementDistance = 100.0f;
	bool ReverseValue = false;
	RoadMonsterDir CurDir = RoadMonsterDir::Left;
	
	class GameEngineCollision* BodyCollision = nullptr;
	class GameEngineRenderer* Renderer = nullptr;
};

