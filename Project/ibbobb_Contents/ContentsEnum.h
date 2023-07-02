#pragma once

enum class UpdateOrder
{
	Player,
	RoadMonster,
	JumpingMonster,
	Medal,
	BackGround,
	Warp,
	Map,
	UI,
};

enum class RenderOrder
{
	BackGround,
	Map,
	Image,

	Medal,
	RoadMonster,
	JumpingMonster,
	MonsterCore,

	ibb,
	obb,

	Warp,
	UI,
};

enum class CollisionOrder
{
	ibbBody,
	ibbLeft,
	ibbRight,
	ibbUp,
	ibbDown,

	obbBody,
	obbLeft,
	obbRight,
	obbUp,
	obbDown,

	MonsterBody,
	MonsterCore,

	Medal,

	LevelDoor,
};
