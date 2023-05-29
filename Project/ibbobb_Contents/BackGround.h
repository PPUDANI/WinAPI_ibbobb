#pragma once
#include <GameEngineCore/GameEngineActor.h>

#include <string>

class BackGround : public GameEngineActor
{
public:
	BackGround();
	~BackGround();

	BackGround(const BackGround& _Other) = delete;
	BackGround(BackGround&& _Other) noexcept = delete;
	BackGround& operator=(const BackGround& _Other) = delete;
	BackGround& operator=(BackGround&& _Other) noexcept = delete;

	void Init(const std::string& _FileName);

protected:

private:
	GameEngineRenderer* Renderer;

	void Start() override;
	void Update(float _Delta) override;
	void Render(float _DeltaTime) override;
	void Release() override;
};

