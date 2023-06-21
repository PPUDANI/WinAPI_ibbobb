#pragma once
#include "Player.h"

class ibb : public Player
{
public:
	// Constructor Destructor
	ibb();
	~ibb();

	// Delete Function
	ibb(const ibb& _Other) = delete;
	ibb(ibb&& _Other) noexcept = delete;
	ibb& operator=(const ibb& _Other) = delete;
	ibb& operator=(ibb&& _Other) noexcept = delete;

protected:

private:
	void Init() override;
	bool PlayerColCheck() override;
	bool PlayerColCheck(std::vector<GameEngineCollision*> _ColVec) override;
	static bool IsLoadTexture;
};