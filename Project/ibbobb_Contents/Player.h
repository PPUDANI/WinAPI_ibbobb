#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class PlayerState
{
	Idle,
	Run,
	Max
};

enum class PlayerDir
{
	Left,
	Right,
	Max
};

class Player : public GameEngineActor
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

	void ChangeState(PlayerState _State);
	std::string SetAnimDir(const std::string);
	PlayerState State = PlayerState::Max;
	PlayerDir Dir = PlayerDir::Right;


	GameEngineRenderer* MainRenderer = nullptr;
};

