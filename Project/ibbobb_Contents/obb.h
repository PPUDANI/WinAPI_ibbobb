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
	bool PlayerColCheck() override;
	bool PlayerColCheck(std::vector<GameEngineCollision*> _ColVec) override;
	static bool IsLoadTexture;
	
};