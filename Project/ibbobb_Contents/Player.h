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
	void SetAnimation(const std::string _State, int _StartFrame = 0);

	void ChangeState(PlayerState _State)
	{
		State = _State;
	}

	// 캐릭터 상태변수
	PlayerState State = PlayerState::Fall;
	PlayerDir CurDir = PlayerDir::Right;
	bool FromJump = false;
	bool FromRun = false;
	

	// 캐릭터 물리변수
	float JumpForce = 1.0f;
	float Speed = 1.0f;
	
	float DefaultGravityPower = 0.0f;
	bool PrevAreaVectorInit = false;
	float4 PrevAreaVector = float4::ZERO;

	float MinGravityInHole = 0.0f;
	float MaxGravityInHole = 0.0f;

	float ErrorRangeOfGravity = 0.0f;
	

	// 맵 충돌 체크
	bool CheckPosOn = false;

	float4 MapLeftUpCheck = { -19.0f, -17.0f };
	float4 MapLeftMiddleCheck = { -19.0f, 0.0f };
	float4 MapLeftDownCheck = { -19.0f, 17.0f };
	
	float4 MapRightUpCheck = { 19.0f, -17.0f };
	float4 MapRightMiddleCheck = { 19.0f, 0.0f };
	float4 MapRightDownCheck = { 19.0f, 17.0f };
	
	float4 MapMiddleUpCheck = { 0.0f, -17.0f };
	float4 MapMiddleDownCheck = { 0.0f, 17.0f };
	
	bool LengthWorpPass = false;
	bool WidthWorpPass = false;
};

