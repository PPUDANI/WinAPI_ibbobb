#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>
enum class MedalState
{
	Idle,
	Get,
	Acquired
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

	void Init();
	void ReverseInit();
protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelStart() override;
	void SetAnimation(const std::string _Name);

	void IdleUpdate(float _DeltaTime);
	void GetUpdate(float _DeltaTime);
	void AcquiredUpdate(float _DeltaTime);
	void MedalArrayInit();

	MedalState CurState;
	class GameEngineRenderer* Renderer = nullptr;
	bool IsAcquired = false;
	bool ReverseValue = false;

	// 메달 물리변수
	float Speed = 0.01f;
	float Radian = 0.0f;
	float MovingHeight = 60.0f;

	static std::vector<Medal*>MedalsByLevelIsAcquired;

};