#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>
enum class WarpType
{
	ibbOnly,
	obbOnly,
	Common
};

enum class WarpDir
{
	Horizontal,
	Vertical
};

class Warp : public GameEngineActor
{
public:
	// Constructor Destructor
	Warp();
	~Warp();

	// Delete Function
	Warp(const Warp& _Other) = delete;
	Warp(Warp&& _Other) noexcept = delete;
	Warp& operator=(const Warp& _Other) = delete;
	Warp& operator=(Warp&& _Other) noexcept = delete;

	inline void SetWorpDir(WarpDir _Dir)
	{
		Dir = _Dir;
	}
	
	inline void SetStarNumber(int _Num)
	{
		StarNum = _Num;
	}

	void Init(WarpType _Type);

protected:

private:

	void Start() override;
	void Update(float _DeltaTime) override;
	void InitPos();

	WarpType Type = WarpType::Common;
	WarpDir Dir;
	std::vector<GameEngineRenderer*> StarRenderer;
	int StarNum = 5;

	
	// Warp 물리변수
	float StarInterval = 13.0f;
	float Speed = 7.0f;
	float Radian = GameEngineMath::PI / 2.0f;
	float MovingHeight = 15.0f;

	class GameEngineCollision* WarpCol = nullptr;
};