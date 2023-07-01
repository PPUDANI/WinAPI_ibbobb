#pragma once

class DoorStar
{
public:
	// Constructor Destructor
	DoorStar();
	~DoorStar();

	// Delete Function
	DoorStar(const DoorStar& _Other) = delete;
	DoorStar(DoorStar&& _Other) noexcept = delete;
	DoorStar& operator=(const DoorStar& _Other) = delete;
	DoorStar& operator=(DoorStar&& _Other) noexcept = delete;

protected:

private:

};