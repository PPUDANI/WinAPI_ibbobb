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
	inline float4 GetRendererScale()
	{
		
	}
protected:

private:
	void Start() override;

	GameEngineRenderer* Renderer = nullptr;
	GameEngineRenderer* DebugRenderer = nullptr;
	bool SwitchRenderValue = true;

};

