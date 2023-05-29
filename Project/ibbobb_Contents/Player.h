#pragma once
#include "GravityActor.h"
#include "PlayerEnum.h"

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
	void Update(float _Delta) override;

	void IdleUpdate(float _Delta);
	void RunUpdate(float _Delta);
	void FallUpdate(float _DeltaTime);
	void JumpUpdate(float _DeltaTime);
	void ChangeState(PlayerState _State);
	void SetAnimation(const std::string);

	PlayerState State = PlayerState::Max;
	PlayerDir Dir = PlayerDir::Right;

	float JumpForce = 1.0f;

	float4 DownCheck = { 0.0f, 19.0f };
	GameEngineRenderer* MainRenderer = nullptr;
};

