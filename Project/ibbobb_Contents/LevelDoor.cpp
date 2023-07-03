#include "LevelDoor.h"
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"
#include "Player.h"
LevelDoor::LevelDoor()
{
}

LevelDoor::~LevelDoor()
{
}

void LevelDoor::Start()
{
	DoorCol = CreateCollision(CollisionOrder::LevelDoor);
	DoorCol->SetCollisionType(CollisionType::Rect);
	DoorCol->SetCollisionPos({ 0.0f, 0.0f });
	DoorCol->SetCollisionScale({ 80.0f, 60.0f });
}

void LevelDoor::Update(float _DeltaTime)
{
	if (true == ActivationValue)
	{
		std::vector<GameEngineCollision*> _ibbCol;
		std::vector<GameEngineCollision*> _obbCol;
		if (true == DoorCol->Collision(CollisionOrder::ibbBody,
			_ibbCol,
			CollisionType::Rect,
			CollisionType::Rect) &&
			true == DoorCol->Collision(CollisionOrder::obbBody,
				_obbCol,
				CollisionType::Rect,
				CollisionType::Rect))
		{
			GameEngineActor* _Actor = _ibbCol[0]->GetActor();
			Player* _ibb = dynamic_cast<Player*>(_Actor);

			_Actor = _obbCol[0]->GetActor();
			Player* _obb = dynamic_cast<Player*>(_Actor);

			if (_ibb->GetState() == PlayerState::Crouch &&
				_obb->GetState() == PlayerState::Crouch)
			{
				PlayerEnter = true;
			}
		}
	}
}
