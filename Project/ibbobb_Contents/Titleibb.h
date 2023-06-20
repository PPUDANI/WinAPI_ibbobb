#pragma once
#include <GameEngineCore/GameEngineActor.h>
class Titleibb : public GameEngineActor
{
public:
	// Constructor Destructor
	Titleibb();
	~Titleibb();

	// Delete Function
	Titleibb(const Titleibb& _Other) = delete;
	Titleibb(Titleibb&& _Other) noexcept = delete;
	Titleibb& operator=(const Titleibb& _Other) = delete;
	Titleibb& operator=(Titleibb&& _Other) noexcept = delete;

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	GameEngineRenderer* ibbRenderer = nullptr;

};