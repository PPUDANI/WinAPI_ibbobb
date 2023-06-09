#pragma once

enum class UpdateOrder
{
	Player,
	RoadMonster,
	JumpingMonster,
	BackGround,
};

enum class RenderOrder
{
	BackGround,
	Map,
	Player,
	RoadMonster,
	JumpingMonster,
	PlayUI,
};