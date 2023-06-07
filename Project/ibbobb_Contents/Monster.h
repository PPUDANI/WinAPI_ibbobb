#pragma once
#include "GravityActor.h"

class Monster : public GravityActor
{
public:
	Monster();
	~Monster();

	Monster(const Monster& _Other) = delete;
	Monster(Monster&& _Other) noexcept = delete;
	Monster& operator=(const Monster& _Other) = delete;
	Monster& operator=(Monster&& _Other) noexcept = delete;
protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	class GameEngineRenderer* Renderer;
};

