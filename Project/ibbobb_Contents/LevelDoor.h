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

	inline void PlayerGoInInit()
	{
		PlayerGoIn = false;
	}

	inline bool IsPlayerGoIn()
	{
		if (PlayerGoIn == true)
		{
			PlayerGoIn = false;
			return true;
		}
		return PlayerGoIn;
	}

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	bool PlayerGoIn = false;
	class GameEngineCollision* DoorCol = nullptr;
};