#pragma once

enum class UpdateOrder
{
	Player,
	RoadMonster,
	JumpingMonster,
	BackGround
};

enum class RenderOrder
{
	BackGround,
	Map,
	Image,
	RoadMonster,
	JumpingMonster,
	ibb,
	obb,
	PlayUI,
};

enum class CollisionOrder
{
	ibb,
	obb,
	MonsterBody,
};