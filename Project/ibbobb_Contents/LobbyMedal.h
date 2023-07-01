#pragma once

class LobbyMedal
{
public:
	// Constructor Destructor
	LobbyMedal();
	~LobbyMedal();

	// Delete Function
	LobbyMedal(const LobbyMedal& _Other) = delete;
	LobbyMedal(LobbyMedal&& _Other) noexcept = delete;
	LobbyMedal& operator=(const LobbyMedal& _Other) = delete;
	LobbyMedal& operator=(LobbyMedal&& _Other) noexcept = delete;

protected:

private:

};