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

	inline void SetWorpType(WarpType _Type)
	{
		Type = _Type;
	}

	inline void SetWorpDir(WarpDir _Dir)
	{
		Dir = _Dir;
	}
	
	inline void SetStarNumber(int _Num)
	{
		StarNum = _Num;
	}

	void Init();
protected:

private:

	void Start() override;
	void Update(float _DeltaTime) override;

	WarpType Type = WarpType::Common;
	WarpDir Dir;
	std::vector<GameEngineRenderer*> StarRenderer;
	int StarNum = 5;

	// Warp 물리변수
	float Speed = 0.01f;
	float Radian = 90.0f;
	float MovingHeight = 8.0f;

};