#pragma once
#include "GravityActor.h"

enum class PlayerState
{
	Idle,
	Run,
	Jump,
	Fall,
	Max
};

enum class PlayerDir
{
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	Max
};

class Player : public GravityActor
{
public:
	
	Player();
	~Player();

	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;


protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void IdleUpdate(float _DeltaTime);
	void RunUpdate(float _DeltaTime);
	void FallUpdate(float _DeltaTime);
	void JumpUpdate(float _DeltaTime);

	void ChangeState(PlayerState _State);
	void SetAnimation(const std::string _State, int _StartFrame = 0);

	void SetJumpForce(float _JumpForce)
	{
		JumpForce = _JumpForce;
	}

	PlayerState State = PlayerState::Max;
	PlayerDir CurDir = PlayerDir::UpRight;

	float JumpForce = 1.0f;
	float4 CurGravity = float4::DOWN;
	float4 DownCheck = { 0.0f, 18.0f };
	GameEngineRenderer* MainRenderer = nullptr;
};

