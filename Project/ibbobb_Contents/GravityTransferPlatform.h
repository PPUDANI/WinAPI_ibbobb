#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEnginePlatform/GameEngineSound.h>

enum class PlatformState
{
	Idle,
	ibbOnThisPlatform,
	obbOnThisPlatform,
	TransferibbToobb,
	TransferobbToibb
};

class GravityTransferPlatform : public GameEngineActor
{
public:
	// Constructor Destructor
	GravityTransferPlatform();
	~GravityTransferPlatform();

	// Delete Function
	GravityTransferPlatform(const GravityTransferPlatform& _Other) = delete;
	GravityTransferPlatform(GravityTransferPlatform&& _Other) noexcept = delete;
	GravityTransferPlatform& operator=(const GravityTransferPlatform& _Other) = delete;
	GravityTransferPlatform& operator=(GravityTransferPlatform&& _Other) noexcept = delete;


protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void IdleUpdate(float _DeltaTime);

	float GetAbsoluteValue(float _Value);
	void SetAnimation(const std::string _Name);

	void ibbOnThisPlatformUpdate(float _DeltaTime);
	void obbOnThisPlatformUpdate(float _DeltaTime);

	void TransferibbToobbUpdate(float _DeltaTime);
	void TransferobbToibbUpdate(float _DeltaTime);
	
	bool ibbOnThisPlatform = false;
	bool obbOnThisPlatform = false;
	float SizeUnit = 26.0f;

	bool TransferValue = false;

	bool TransferVectorInit = false;
	float4 TransferVector = float4::ZERO;

	PlatformState State;

	GameEngineCollision* PlatformCol = nullptr;

	GameEngineRenderer* PlatformRenderer = nullptr;

	GameEngineSoundPlayer EffectPlayer;
};