#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEnginePlatform/GameEngineSound.h>
class ibb;
class obb;
class SubLevel : public GameEngineLevel
{
public:
	// Constructor Destructor
	SubLevel();
	~SubLevel();

	// Delete Function
	SubLevel(const SubLevel& _Other) = delete;
	SubLevel(SubLevel&& _Other) noexcept = delete;
	SubLevel& operator=(const SubLevel& _Other) = delete;
	SubLevel& operator=(SubLevel&& _Other) noexcept = delete;

	static void DevelopmentModeOn()
	{
		DevelopmentMode = true;
	}

	static bool IsDevelopmentMode()
	{
		return DevelopmentMode;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void ReviveCharacter();

	float4 WindowScale = float4::ZERO;
	float ZoomScale = 1.5f;

	class BackGround* Back = nullptr;
	class Map* LevelMap = nullptr;
	float LevelMaxScaleX = 0.0f;
	class ibb* ibbPlayer = nullptr;
	class obb* obbPlayer = nullptr;
	bool FirstDeathIsibb = false;

	static bool DevelopmentMode;
private:
	
};