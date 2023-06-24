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

	bool LeftToOtherRightCheck() override;
	bool RightToOtherLeftCheck() override;
	bool UpToOtherDownCheck() override;
	bool DownToOtherUpCheck() override;

	bool UpToOtherBodyCheck() override;
	bool DownToOtherBodyCheck() override;
};