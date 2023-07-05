#pragma once
#include "GravityActor.h"

#include <GameEnginePlatform/GameEngineSound.h>

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
	Turn,
	Dead,
	Live
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

	void Init(const float4& _InitPos);
	void ReverseInit(const float4& _InitPos);

	inline void SetDir(RoadMonsterDir _Dir)
	{
		CurDir = _Dir;
	}

	inline void ChangeState(RoadMonsterState _State)
	{
		CurState = _State;
	}

	void SetAnimation(const std::string _State, int _StartFrame = 0);
	void SetCoreAnimation(const std::string _State);
protected:
	
private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void MoveUpdate(float _DeltaTime);
	void DeadUpdate(float _DeltaTime);
	void TurnUpdate(float _DeltaTime);
	void LiveUpdate(float _DeltaTime);
	
	bool MovePossibleCheck();


	// RoadMonster 상태변수
	bool ReverseValue = false;
	bool IsAnimBlink = false;
	bool CheckPosOn = false;
	RoadMonsterDir CurDir = RoadMonsterDir::Left;
	RoadMonsterState CurState;

	// RoadMonster 물리변수
	float Speed = 100.0f;
	float CurDistance = 0.0f;
	float4 StartVector = float4::ZERO;

	float4 LeftCheck = { -30.0f, 0.0f };
	float4 RightCheck = { 30.0f, 0.0f };
	float4 LeftDownCheck = { -55.0f, 12.0f };
	float4 RightDownCheck = { 55.0f, 12.0f };

	float4 ReverseLeftDownCheck = { -55.0f, -12.0f };
	float4 ReverseRightDownCheck = { 55.0f, -12.0f };

	class GameEngineRenderer* MonsterRenderer = nullptr;
	class GameEngineRenderer* CoreRenderer = nullptr;

	class GameEngineCollision* BodyCollision = nullptr;
	class GameEngineCollision* CoreCollision = nullptr;

	GameEngineSoundPlayer EffectPlayer;
};

