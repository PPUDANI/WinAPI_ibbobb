#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class FadeState
{
	FadeIn,
	FadeOut
};

class Fade : public GameEngineActor
{
public:
	// Constructor Destructor
	Fade();
	~Fade();

	// Delete Function
	Fade(const Fade& _Other) = delete;
	Fade(Fade&& _Other) noexcept = delete;
	Fade& operator=(const Fade& _Other) = delete;
	Fade& operator=(Fade&& _Other) noexcept = delete;

	float GetValue() const
	{
		return AlphaValue;
	}

	inline void SetFadeSpeed(float _Speed)
	{
		FadeSpeed = _Speed;
	}

	inline bool FadeIsEnd() const
	{
		return FadeEnd;
	}

	inline void FadeStop()
	{
		FadeEnd = true;
	}

	void Init(const std::string& _FileName, FadeState _State);
protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	class GameEngineRenderer* Renderer = nullptr;

	float FadeSpeed = 100.0f;
	float AlphaValue = 0.0f;

	FadeState State = FadeState::FadeOut;
	bool FadeEnd = false;
};