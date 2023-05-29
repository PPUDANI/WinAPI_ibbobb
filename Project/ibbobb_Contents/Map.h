#pragma once
#include <GameEngineCore/GameEngineActor.h>


class Map : public GameEngineActor
{
public:
	Map();
	~Map();

	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

	void Init(const std::string& _FileName, const std::string& _DebugFileName);
	void SwitchRender();
protected:

private:
	GameEngineRenderer* Renderer = nullptr;
	GameEngineRenderer* DebugRenderer = nullptr;
	bool SwitchRenderValue = true;
	

	void Start() override;
	void Update(float _Delta) override;
	void Render(float _DeltaTime) override;
	void Release() override;
};

