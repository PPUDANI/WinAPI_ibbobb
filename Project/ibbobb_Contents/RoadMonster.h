#pragma once
#include "GravityActor.h"

enum class RoadMonsterDir
{
	Left,
	Right,
	ReverseLeft,
	ReverseRight
};

enum class RoadMonsterState
{
	Move,
	Dead
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

	void Init();
	void ReverseInit();

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
	void MoveUpdate(float _DeltaTime);
	void DeadUpdate(float _DeltaTime);
	void SetAnimation(const std::string _State, int _StartFrame = 0);

	inline void ChangeState(RoadMonsterState _State)
	{
		CurState = _State;
	}

	// RoadMonster 상태변수
	bool ReverseValue = false;
	bool IsAnimBlink = false;
	RoadMonsterDir CurDir = RoadMonsterDir::Left;
	RoadMonsterState CurState;

	// RoadMonster 물리변수
	float Speed = 0.1f;
	float CurDistance = 0.0f;
	float MovementDistance = 100.0f;

	class GameEngineRenderer* MonsterRenderer = nullptr;
	class GameEngineRenderer* CoreRenderer = nullptr;

	class GameEngineCollision* BodyCollision = nullptr;
	class GameEngineCollision* CoreCollision = nullptr;

};

