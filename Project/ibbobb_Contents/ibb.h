#pragma once
#include "Player.h"

class ibb : public Player
{
private:
	static ibb* Mainibb;
public:
	static ibb* GetMainibb()
	{
		return Mainibb;
	}
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

	bool LeftToOtherRightCheck() override;
	bool RightToOtherLeftCheck() override;
	bool UpToOtherDownCheck() override;
	bool DownToOtherUpCheck() override;

	bool UpToOtherBodyCheck() override;
	bool DownToOtherBodyCheck() override;

	void LevelStart() override;
};