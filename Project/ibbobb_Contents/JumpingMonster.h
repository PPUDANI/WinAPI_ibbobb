#pragma once

#include "GravityActor.h"

enum class JumpingMonsterState
{
	Fall,
	Dead
};

enum class JumpingMonsterDir
{
	Default,
	Reverse
};

class JumpingMonster : public GravityActor
{
public:
	// Constructor Destructor
	JumpingMonster();
	~JumpingMonster();

	// Delete Function
	JumpingMonster(const JumpingMonster& _Other) = delete;
	JumpingMonster(JumpingMonster&& _Other) noexcept = delete;
	JumpingMonster& operator=(const JumpingMonster& _Other) = delete;
	JumpingMonster& operator=(JumpingMonster&& _Other) noexcept = delete;

	inline void SetJumpForce(float _JumpForce)
	{
		JumpForce = _JumpForce;
	}

	inline void Reverse(float _JumpForce)
	{
		JumpForce = _JumpForce;
	}

protected:

private:
	
	void Start() override;
	void Update(float _DeltaTime) override;
	void FallUpdate(float _DeltaTime);

	void DeadUpdate(float _DeltaTime);
	void SetAnimation(const std::string _State);

	inline void ChangeState(JumpingMonsterState _State)
	{
		CurState = _State;
	}


	class GameEngineRenderer* MonsterRenderer = nullptr;
	class GameEngineRenderer* CoreRenderer = nullptr;

	class GameEngineCollision* CoreCollision = nullptr;
	class GameEngineCollision* BodyCollision;

	float JumpForce = 1.0f;
	bool ReverseValue = false;
	bool IsAnimBlink = false;
	float PrevPos = 0.0f;
	float MovedPos = 0.0f;
	float4 AddCoreVec = float4::ZERO;

	float BodyHalf = 35.0f;
	float BlockUnitHeight = 26.0f;
	float4 DownCheck;

	JumpingMonsterState CurState;
};