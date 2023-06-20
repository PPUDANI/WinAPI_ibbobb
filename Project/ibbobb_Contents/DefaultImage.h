#pragma once

#include <GameEngineCore/GameEngineActor.h>

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

	void Init(const std::string& _FileName);

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	GameEngineRenderer* Renderer;
};