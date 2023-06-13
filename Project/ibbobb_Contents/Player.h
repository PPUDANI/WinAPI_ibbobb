#pragma once
#include "GravityActor.h"

enum class PlayerState
{
	Idle,
	Blink,
	Run,
	Jump,
	Fall,
	Dead,
	Crouch
};

enum class PlayerDir
{
	Left,
	Right,
	ReverseLeft,
	ReverseRight
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

	virtual void Init() {}

	// 렌더러
	GameEngineRenderer* MainRenderer = nullptr;

	// 충돌체
	GameEngineCollision* BodyCollision = nullptr;

	float Ratio = 2.0f;

	int MoveRightKey;
	int MoveLeftKey;
	int JumpKey;
	int CrouchKey;

private:

	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void IdleUpdate(float _DeltaTime);
	void RunUpdate(float _DeltaTime);
	void FallUpdate(float _DeltaTime);
	void JumpUpdate(float _DeltaTime);
	void CrouchUpdate(float _DeltaTime);

	void DeadUpdate(float _DeltaTime);
	void ChangeState(PlayerState _State);
	void SetAnimation(const std::string _State, int _StartFrame = 0);

	void ReverseDir();


	void SetJumpForce(float _JumpForce)
	{
		JumpForce = _JumpForce;
	}

	void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	PlayerState State;
	PlayerDir CurDir = PlayerDir::Right;
	bool CurReverse = false;

	bool IsJump = false;
	float JumpForce = 1.0f;
	float Speed = 1.0f;
	float4 CurGravity = float4::DOWN;
	bool CheckPosOn = false;

	// 맵 충돌 체크
	float4 MapLeftUpCheck = { -19.0f, -17.0f };
	float4 MapLeftMiddleCheck = { -19.0f, 0.0f };
	float4 MapLeftDownCheck = { -19.0f, 18.0f };
	
	float4 MapRightUpCheck = { 20.0f, -17.0f };
	float4 MapRightMiddleCheck = { 20.0f, 0.0f };
	float4 MapRightDownCheck = { 20.0f, 18.0f };
	
	float4 MapMiddleUpCheck = { 0.0f, -17.0f };
	float4 MapMiddleDownCheck = { 0.0f, 18.0f };
	
};

