#pragma once

#include "GravityActor.h"
class JumpingMonster : public GravityActor
{
public:
	// Constructor Destructor
	JumpingMonster();
	~JumpingMonster();

	// Delete Function
	JumpingMonster(const JumpingMonster& _Other) = delete;
	JumpingMonster(JumpingMonster&& _Other) noexcept = delete;
	JumpingMonster& operator=(const JumpingMonster& _Other) = delete;
	JumpingMonster& operator=(JumpingMonster&& _Other) noexcept = delete;

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	inline void SetJumpForce(float _JumpForce)
	{
		JumpForce = _JumpForce;
	}

	float JumpForce = 1.0f;
	class GameEngineRenderer* Renderer;
};