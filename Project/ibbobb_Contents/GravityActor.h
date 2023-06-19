#pragma once
#include <GameEngineCore/GameEngineActor.h>

class GravityActor : public GameEngineActor
{
public:
	GravityActor();
	~GravityActor();

	GravityActor(const GravityActor& _Other) = delete;
	GravityActor(GravityActor&& _Other) noexcept = delete;
	GravityActor& operator=(const GravityActor& _Other) = delete;
	GravityActor& operator=(GravityActor&& _Other) noexcept = delete;

	inline void GravityOn()
	{
		GravityValue = true;
	}

	inline void GravityOff()
	{
		GravityValue = false;
	}

	void CameraFocus();
	float4 ActorCameraPos();

	void Gravity(float _DeltaTime);

	void SetGroundTexture(const std::string& _GroundTextureName);
	int GetGroundColor(unsigned int _DefaultColor, float4 _Pos = float4::ZERO);
	
	inline void SetGravityVector(float4 _GravityVector)
	{
		GravityVector = _GravityVector;
	}

	inline float4 GetGravityVector() const
	{
		return GravityVector;
	}

	inline void AddGravityVector(float4 _GravityVector)
	{
		GravityVector += _GravityVector;
	}

	inline void SetGravityPower(float _GravityPower)
	{
		GravityPower = _GravityPower;
	}


	inline float GetGravityPower()
	{
		return GravityPower;
	}

	inline void GravityReset()
	{
		GravityVector = float4::ZERO;
	}

	

protected:
	bool ReverseValue = false;

private:
	class GameEngineWindowTexture* GroundTexture = nullptr;

	bool GravityValue = true;
	float4 GravityVector = float4::ZERO;

	float GravityPower = 0.0f;
};

