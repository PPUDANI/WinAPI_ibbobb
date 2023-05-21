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

	void Init(const std::string& _FileName, float _Ratio);
protected:

private:
	void Start() override;
	void Update(float _Delta) override;
	void Render() override;
	void Release() override;
};

