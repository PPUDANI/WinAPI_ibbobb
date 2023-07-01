#pragma once
#include <GameEngineCore/GameEngineActor.h>

class LobbyMedal : public GameEngineActor
{
public:
	// Constructor Destructor
	LobbyMedal();
	~LobbyMedal();

	// Delete Function
	LobbyMedal(const LobbyMedal& _Other) = delete;
	LobbyMedal(LobbyMedal&& _Other) noexcept = delete;
	LobbyMedal& operator=(const LobbyMedal& _Other) = delete;
	LobbyMedal& operator=(LobbyMedal&& _Other) noexcept = delete;

	void SetAnimation(const std::string _Name);

protected:

private:
	void Start() override;
	void LevelStart() override;

	class GameEngineRenderer* Renderer;
};