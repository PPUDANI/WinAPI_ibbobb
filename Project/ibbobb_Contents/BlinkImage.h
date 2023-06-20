#pragma once
#include <GameEngineCore/GameEngineActor.h>

class BlinkImage : public GameEngineActor
{
public:
	// Constructor Destructor
	BlinkImage();
	~BlinkImage();

	// Delete Function
	BlinkImage(const BlinkImage& _Other) = delete;
	BlinkImage(BlinkImage&& _Other) noexcept = delete;
	BlinkImage& operator=(const BlinkImage& _Other) = delete;
	BlinkImage& operator=(BlinkImage&& _Other) noexcept = delete;

	void Init(const std::string& _FileName);

protected:

private:

	GameEngineRenderer* Renderer;
};