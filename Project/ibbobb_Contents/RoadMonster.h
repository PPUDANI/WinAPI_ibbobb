#pragma once
#include "GravityActor.h"

class RoadMonster : public GravityActor
{
public:
	RoadMonster();
	~RoadMonster();

	RoadMonster(const RoadMonster& _Other) = delete;
	RoadMonster(RoadMonster&& _Other) noexcept = delete;
	RoadMonster& operator=(const RoadMonster& _Other) = delete;
	RoadMonster& operator=(RoadMonster&& _Other) noexcept = delete;
protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	class GameEngineCollision* BodyCollision;
	class GameEngineRenderer* Renderer;
};

