#pragma once
#include "GravityActor.h"
#include <vector>
enum class PlayerState
{
	Idle,
	Blink,
	Run,
	Jump,
	Fall,
	Dead,
	Crouch,
	RidingMode
};

enum class PlayerDir
{
	Left,
	Right
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

	inline void SetRatio(float _Ratio)
	{
		Ratio = _Ratio;
	}

	inline float4 GetMovePos() const
	{
		return MovePos;
	}

	inline PlayerDir GetDir() const
	{
		return CurDir;
	}

	inline PlayerState GetState() const
	{
		return CurState;
	}

	inline bool GetReverseValue() const
	{
		return ReverseValue;
	}

	inline void SetPushForce(float _PushForce)
	{
		PushForce = _PushForce;
	}

	inline void SetOtherPlayer(Player* _OtherPlayer)
	{
		OtherPlayer = _OtherPlayer;
	}

	inline Player* GetOtherPlayer() const
	{
		return OtherPlayer;
	}

protected:

	virtual void Init() {}


	// 렌더러
	GameEngineRenderer* MainRenderer = nullptr;
	float Ratio = 2.0f;

	// 충돌체

	virtual bool LeftToOtherRightCheck()
	{
		return false;
	}

	virtual bool RightToOtherLeftCheck()
	{
		return false;
	}

	virtual bool UpToOtherDownCheck()
	{
		return false;
	}

	virtual bool DownToOtherUpCheck()
	{
		return false;
	}

	virtual bool UpToOtherBodyCheck()
	{
		return false;
	}

	virtual bool DownToOtherBodyCheck()
	{
		return false;
	}

	Player* OtherPlayer = nullptr;
	GameEngineCollision* BodyCol = nullptr;
	GameEngineCollision* LeftCol = nullptr;
	GameEngineCollision* RightCol = nullptr;
	GameEngineCollision* UpCol = nullptr;
	GameEngineCollision* DownCol = nullptr;

	int OtherPlayerBodyCol;
	int OtherPlayerLeftCol;
	int OtherPlayerRightCol;
	int OtherPlayerUpCol;
	int OtherPlayerDownCol;
	bool OtherPlayerReverseValue = false;

	// 조작키
	int MoveRightKey = 0;
	int MoveLeftKey = 0;
	int JumpKey = 0;
	int CrouchKey = 0;

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
	void RidingModeUpdate(float _DeltaTime);

	void IdleInitFromFall();
	bool LeftMapColCheck();
	bool RightMapColCheck();
	bool DownMapColCheck();
	bool UpMapColCheck();

	void ReverseInit();
	void ReverseCol();

	
	void OtherPlayerMoveCheck();

	void SetAnimation(const std::string _State, int _StartFrame = 0);
	inline void ChangeState(PlayerState _State)
	{
		CurState = _State;
	}
	

	// 캐릭터 상태변수
	PlayerState CurState = PlayerState::Fall;
	PlayerDir CurDir = PlayerDir::Right;
	bool FromJump = false;
	bool FromRun = false;
	bool AnimIsBlink = false;

	// 캐릭터 물리변수
	float JumpForce = 1.0f;
	float Speed = 1.0f;
	float PushForce = 0.0f;

	float DefaultGravityPower = 0.0f;
	bool PrevAreaVectorInit = false;
	float4 PrevAreaVector = float4::ZERO;

	float MinGravityInHole = 0.0f;
	float MaxGravityInHole = 0.0f;
	float PushPowerInVirticalHole = 0.0f;
	float ErrorRangeOfGravity = 0.0f;

	float4 MovePos = float4::ZERO;
	float4 DistanceFromOtherPlayer = float4::ZERO;

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
	
	bool HorizontalWorpPass = false;
	bool VirticalWorpPass = false;
};

