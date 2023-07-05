#pragma once

enum class UpdateOrder
{
	Player,
	RoadMonster,
	JumpingMonster,
	Medal,
	BackGround,
	Warp,
	GravityPlatform,
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
	GravityPlatform,
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

	ibbOnlyWarp,
	obbOnlyWarp,
	GravityPlatform,
	Medal,

	LevelDoor,
};
