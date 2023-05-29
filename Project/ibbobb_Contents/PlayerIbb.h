#pragma once
#include "GravityActor.h"
#include "PlayerEnum.h"

class PlayerIbb : public GravityActor
{
public:

	PlayerIbb();
	~PlayerIbb();

	PlayerIbb(const PlayerIbb& _Other) = delete;
	PlayerIbb(PlayerIbb&& _Other) noexcept = delete;
	PlayerIbb& operator=(const PlayerIbb& _Other) = delete;
	PlayerIbb& operator=(PlayerIbb&& _Other) noexcept = delete;


protected:

private:
	void Start() override;
	void Update(float _Delta) override;

	void IdleUpdate(float _Delta);
	void RunUpdate(float _Delta);
	void FallUpdate(float _DeltaTime);

	void ChangeState(PlayerState _State);
	void SetAnimation(const std::string);
	PlayerState State = PlayerState::Max;
	PlayerDir Dir = PlayerDir::Right;

	float4 DownCheck = { 0.0f, 18.0f };
	GameEngineRenderer* MainRenderer = nullptr;
};

