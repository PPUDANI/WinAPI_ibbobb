#pragma once

enum class UpdateOrder
{
	Player,
	RoadMonster,
	JumpingMonster,
	BackGround,
	Warp,
	Map
};

enum class RenderOrder
{
	BackGround,
	Map,
	Image,
	RoadMonster,
	JumpingMonster,
	MonsterCore,
	ibb,
	obb,
	Warp,
	PlayUI,
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
};
