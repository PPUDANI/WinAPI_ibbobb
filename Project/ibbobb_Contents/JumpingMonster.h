#pragma once

#include "GravityActor.h"
#include <GameEnginePlatform/GameEngineSound.h>
enum class JumpingMonsterState
{
	Fall,
	Dead,
	Live
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

	inline void ChangeState(JumpingMonsterState _State)
	{
		CurState = _State;
	}

	void Init(const float4& _InitPos);
	void ReverseInit(const float4& _InitPos);

	void SetAnimation(const std::string _State);
	void SetCoreAnimation(const std::string _State);
protected:

private:
	
	void Start() override;
	void Update(float _DeltaTime) override;
	void FallUpdate(float _DeltaTime);
	void DeadUpdate(float _DeltaTime);
	void LiveUpdate(float _DeltaTime);



	class GameEngineRenderer* MonsterRenderer = nullptr;
	class GameEngineRenderer* CoreRenderer = nullptr;

	class GameEngineCollision* CoreCollision = nullptr;
	class GameEngineCollision* BodyCollision = nullptr;


	// Monster 상태 변수
	JumpingMonsterState CurState;
	bool ReverseValue = false;
	bool IsAnimBlink = false;
	
	// Monster 물리 변수
	float JumpForce = 1.0f;
	float BodyHalf = 35.0f;
	float BlockUnitHeight = 26.0f;
	float4 GravityDir = float4::UP;
	float4 DownCheck;
	float4 StartVector = float4::ZERO;

	// Core위치 연산에 필요한 변수
	float PrevPos = 0.0f;
	float MovedPos = 0.0f;
	float4 AddCoreVec = float4::ZERO;

	GameEngineSoundPlayer EffectPlayer;
};