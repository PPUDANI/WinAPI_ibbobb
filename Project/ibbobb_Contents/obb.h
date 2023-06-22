#pragma once
#include "Player.h"

class obb : public Player
{
public:
	// Constructor Destructor
	obb();
	~obb();

	// Delete Function
	obb(const obb& _Other) = delete;
	obb(obb&& _Other) noexcept = delete;
	obb& operator=(const obb& _Other) = delete;
	obb& operator=(obb&& _Other) noexcept = delete;

protected:
	
private:
	void Init() override;

	bool BodyToOtherBodyCheck() override;
	bool LeftToOtherBodyCheck() override;
	bool RightToOtherBodyCheck() override;
	bool UpToOtherBodyCheck() override;
	bool DownToOtherBodyCheck() override;
};