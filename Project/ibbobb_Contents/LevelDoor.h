#pragma once
#include <GameEngineCore/GameEngineActor.h>
class LevelDoor : public GameEngineActor
{
public:
	// Constructor Destructor
	LevelDoor();
	~LevelDoor();

	// Delete Function
	LevelDoor(const LevelDoor& _Other) = delete;
	LevelDoor(LevelDoor&& _Other) noexcept = delete;
	LevelDoor& operator=(const LevelDoor& _Other) = delete;
	LevelDoor& operator=(LevelDoor&& _Other) noexcept = delete;

	inline void DeActivation()
	{
		ActivationValue = false;
	}

	inline void PlayerEnterInit()
	{
		PlayerEnter = false;
	}

	inline bool IsPlayerEnter()
	{
		if (PlayerEnter == true)
		{
			PlayerEnter = false;
			return true;
		}
		return PlayerEnter;
	}

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	bool PlayerEnter = false;
	bool ActivationValue = true;

	class GameEngineCollision* DoorCol = nullptr;
};