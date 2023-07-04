#pragma once
#include <GameEngineCore/GameEngineActor.h>
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
	float SizeUnit = 26.0f;
};