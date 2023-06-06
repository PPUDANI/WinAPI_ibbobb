#pragma once
#include "GravityActor.h"

enum class PlayerState
{
	Idle,
	Blink,
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
	void Render(float _DeltaTime) override;
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

	void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	PlayerState State = PlayerState::Max;
	PlayerDir CurDir = PlayerDir::UpRight;

	float Ratio = 2.0f;

	float JumpForce = 1.0f;
	float Speed = 1.0f;
	float4 CurGravity = float4::DOWN;

	GameEngineRenderer* MainRenderer = nullptr;

	// ¸Ê Ãæµ¹ Ã¼Å©
	//float4 MapLeftUpCheck = { -18.0f, -18.0f };
	//float4 MapLeftCenterCheck = { -18.0f, 0.0f };
	//float4 MapLeftDownCheck = { -18.0f, 18.0f };
	//
	//float4 MapRightUpCheck = { 18.0f, -18.0f };
	//float4 MapRightCenterCheck = { 18.0f, 0.0f };
	//float4 MapRightDownCheck = { 18.0f, 18.0f };
	//
	//float4 MapUpCenterCheck = { 0.0f, -18.0f };
	//float4 MapDownCenterCheck = { 0.0f, 18.0f };

	float4 MapLeftUpCheck = { -9.0f, -9.0f };
	float4 MapLeftCenterCheck = { -9.0f, 0.0f };
	float4 MapLeftDownCheck = { -9.0f, 9.0f };
	
	float4 MapRightUpCheck = { 9.0f, -9.0f };
	float4 MapRightCenterCheck = { 9.0f, 0.0f };
	float4 MapRightDownCheck = { 9.0f, 9.0f };
	
	float4 MapUpCenterCheck = { 0.0f, -9.0f };
	float4 MapDownCenterCheck = { 0.0f, 9.0f };
};

