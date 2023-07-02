#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class ImageState
{
	Idle,
	Levitation
};

class DefaultImage : public GameEngineActor
{
public:
	// Constructor Destructor
	DefaultImage();
	~DefaultImage();

	// Delete Function
	DefaultImage(const DefaultImage& _Other) = delete;
	DefaultImage(DefaultImage&& _Other) noexcept = delete;
	DefaultImage& operator=(const DefaultImage& _Other) = delete;
	DefaultImage& operator=(DefaultImage&& _Other) noexcept = delete;

	inline void LevitationOn()
	{
		LevitationValue = true;
	}

	inline void LevitationOff()
	{
		LevitationValue = false;
	}

	inline void BlinkOn()
	{
		BlinkValue = true;
	}

	inline void BlinkOff()
	{
		BlinkValue = false;
	}

	void Init(const std::string& _FileName, float _BlinkFrameSpeed = 0.5f);

protected:

private:
	void Update(float _DeltaTime) override;
	void BlinkUpdate(float _DeltaTime);
	void LevitationUpdate(float _DeltaTime);
	void LevelStart() override;
	class GameEngineRenderer* Renderer;
	bool BlinkValue = false;
	bool LevitationValue = false;

	// Image 물리변수
	float Speed = 4.0f;
	float Radian = GameEngineMath::PI / 2.0f;
	float MovingHeight = 30.0f;
	float4 StartPos = float4::ZERO;

};