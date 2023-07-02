#pragma once
#include "DefaultImage.h"
class LobbyLevelText : public DefaultImage
{
public:
	// Constructor Destructor
	LobbyLevelText();
	~LobbyLevelText();

	// Delete Function
	LobbyLevelText(const LobbyLevelText& _Other) = delete;
	LobbyLevelText(LobbyLevelText&& _Other) noexcept = delete;
	LobbyLevelText& operator=(const LobbyLevelText& _Other) = delete;
	LobbyLevelText& operator=(LobbyLevelText&& _Other) noexcept = delete;

	void Init(const std::string _FileName, float _BlinkFrameSpeed = 0.5f);

	void Activation()
	{
		DeActivationValue = false;
	}

protected:
	void Update(float _DeltaTime) override;
private:
	
	bool DeActivationValue = true;
};