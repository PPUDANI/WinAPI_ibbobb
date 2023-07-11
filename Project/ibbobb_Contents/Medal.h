#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include <vector>

enum class MedalState
{
	Idle,
	Get
};

class Medal : public GameEngineActor
{
public:
	// Constructor Destructor
	Medal();
	~Medal();

	// Delete Function
	Medal(const Medal& _Other) = delete;
	Medal(Medal&& _Other) noexcept = delete;
	Medal& operator=(const Medal& _Other) = delete;
	Medal& operator=(Medal&& _Other) noexcept = delete;

	static std::vector<Medal*>MedalsByLevelIsAcquired;

	void Init();
	void ReverseInit();

	inline bool IsAcquired() const
	{
		return AcquiredValue;
	}

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelStart() override;
	void SetAnimation(const std::string _Name);

	void IdleUpdate(float _DeltaTime);
	void GetUpdate(float _DeltaTime);
	void Levitation(float _DeltaTime);
	
	class GameEngineRenderer* Renderer = nullptr;

	class GameEngineCollision* MedalCol = nullptr;

	// Medal 상태변수
	MedalState CurState;
	bool AcquiredValue = false;
	bool ReverseValue = false;
	int Count = 0;

	// Medal 물리변수
	float Speed = 5.0f;
	float Radian = GameEngineMath::PI / 2.0f;
	float MovingHeight = 30.0f;
	float4 StartPos = float4::ZERO;

	GameEngineSoundPlayer EffectPlayer;
};