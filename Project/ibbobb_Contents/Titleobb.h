#pragma once
#include <GameEngineCore/GameEngineActor.h>
class Titleobb : public GameEngineActor
{
public:
	// Constructor Destructor
	Titleobb();
	~Titleobb();

	// Delete Function
	Titleobb(const Titleobb& _Other) = delete;
	Titleobb(Titleobb&& _Other) noexcept = delete;
	Titleobb& operator=(const Titleobb& _Other) = delete;
	Titleobb& operator=(Titleobb&& _Other) noexcept = delete;

protected:

private:

	void Start() override;
	void Update(float _DeltaTime) override;

	GameEngineRenderer* obbRenderer = nullptr;
};