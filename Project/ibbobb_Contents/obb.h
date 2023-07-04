#pragma once
#include "Player.h"

class obb : public Player
{
private:
	static obb* Mainobb;

public:
	static obb* GetMainobb()
	{
		return Mainobb;
	}

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

	bool LeftToOtherColorWarpCheck() override;
	bool RightToOtherColorWarpCheck() override;
	bool UpToOtherColorWarpCheck() override;
	bool DownToOtherColorWarpCheck() override;

	void LevelStart() override;
};