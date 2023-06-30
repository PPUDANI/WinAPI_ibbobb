#pragma once
#include <GameEngineCore/GameEngineLevel.h>
class Lobby : public GameEngineLevel
{
public:
	// Constructor Destructor
	Lobby();
	~Lobby();

	// Delete Function
	Lobby(const Lobby& _Other) = delete;
	Lobby(Lobby&& _Other) noexcept = delete;
	Lobby& operator=(const Lobby& _Other) = delete;
	Lobby& operator=(Lobby&& _Other) noexcept = delete;

protected:
	void LevelStart(GameEngineLevel* _PrevLevel)override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	class BackGround* Back = nullptr;
	class Map* LobbyMap = nullptr;


};