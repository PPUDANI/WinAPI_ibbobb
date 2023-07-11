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


private:
	// Actor ��� �Լ�
	void Start() override;
	void Update(float _DeltaTime) override;


private:
	// ���� �Լ�
	void IdleUpdate(float _DeltaTime);

	void ibbOnThisPlatformUpdate(float _DeltaTime);
	void obbOnThisPlatformUpdate(float _DeltaTime);

	void TransferibbToobbUpdate(float _DeltaTime);
	void TransferobbToibbUpdate(float _DeltaTime);

	// ���� ����
	PlatformState State;

	bool ibbOnThisPlatform = false;
	bool obbOnThisPlatform = false;
	bool TransferValue = false;
	bool TransferVectorInit = false;


private:
	// ���� �Լ�
	float GetAbsoluteValue(float _Value);
	void SetAnimation(const std::string _Name);


private:
	// ���� ����
	float SizeUnit = 26.0f;
	float4 TransferVector = float4::ZERO;

	
private:
	// ��� ����
	GameEngineCollision* PlatformCol = nullptr;
	GameEngineRenderer* PlatformRenderer = nullptr;
	GameEngineSoundPlayer EffectPlayer;
};