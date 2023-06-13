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
	ibb,
	obb,
	RoadMonster,
	JumpingMonster,
	PlayUI,
};

enum class CollisionOrder
{
	PlayerBody,
	MonsterBody,
};