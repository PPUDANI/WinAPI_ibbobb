#pragma once
class GameEngineMath
{
};

class float4
{
public:
	float X;
	float Y;
	float Z;
	float W;

	inline int iX() const
	{
		return static_cast<int>(X);
	}
	inline int iY() const
	{
		return static_cast<int>(Y);
	}
	inline int ihX() const
	{
		return static_cast<int>(X);
	}
	inline int ihY() const
	{
		return static_cast<int>(Y);
	}
};