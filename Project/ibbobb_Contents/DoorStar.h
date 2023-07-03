#pragma once
#include <GameEngineCore/GameEngineActor.h>
class DoorStar : public GameEngineActor
{
public:
	// Constructor Destructor
	DoorStar();
	~DoorStar();

	// Delete Function
	DoorStar(const DoorStar& _Other) = delete;
	DoorStar(DoorStar&& _Other) noexcept = delete;
	DoorStar& operator=(const DoorStar& _Other) = delete;
	DoorStar& operator=(DoorStar&& _Other) noexcept = delete;

	void Init();
	inline void Activation()
	{
		ActivationValue = true;
	}

protected:
	
	

private:
	void Update(float _DeltaTime) override;

	bool ActivationValue = false;
	class GameEngineRenderer* Renderer;
};