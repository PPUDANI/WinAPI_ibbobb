#include "Player.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"


Player::Player()
{

}

Player::~Player()
{

}

void Player::Start()
{
	Init();

	ChangeState(PlayerState::Fall);

	// 첫 방향 설정
	CurDir = PlayerDir::Right;

	// 이건 무슨 함수?
	//MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(Ratio);

	// 중력값 설정
	DefaultGravityPower = 1200.0f;
	SetGravityPower(DefaultGravityPower);

	// 워프홀 기본 중력값 설정 (중력값이 작아 못 빠져나올 가능성 제거)
	MinGravityInHole = 350.0f;
	MaxGravityInHole = 1100.0f;
	ErrorRangeOfGravity = 1.1f;
	PushPowerInVirticalHole = 50.0f;

	// 점프력 설정
	JumpForce = 550.0f;

	// 속도 설정
	Speed = 200.0f;

}

void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		CheckPosOn = !CheckPosOn;
	}

	// 가로워프, 세로워프 구분
	{
		unsigned int MiddleColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
		unsigned int LeftColor = GetGroundColor(RGB(0, 255, 255), MapLeftMiddleCheck);
		unsigned int RightColor = GetGroundColor(RGB(0, 255, 255), MapRightMiddleCheck);
		unsigned int UpColor = GetGroundColor(RGB(0, 255, 255), MapMiddleUpCheck);
		unsigned int DownColor = GetGroundColor(RGB(0, 255, 255), MapMiddleDownCheck);

		if (LeftColor != RightColor)
		{

			HorizontalWorpPass = false;
			VirticalWorpPass = true;
			if (LeftColor == MiddleColor)
			{
				if (true == LeftToOtherRightCheck())
				{
					OtherPlayer->AddPos(float4::LEFT * PushPowerInVirticalHole * _DeltaTime);
				}
				AddPos(float4::LEFT * PushPowerInVirticalHole * _DeltaTime);
			}
			else if (RightColor == MiddleColor)
			{
				if (true == RightToOtherLeftCheck())
				{
					OtherPlayer->AddPos(float4::RIGHT * PushPowerInVirticalHole * _DeltaTime);
				}
				AddPos(float4::RIGHT * PushPowerInVirticalHole * _DeltaTime);
			}
			
		}
		else if (UpColor != DownColor)
		{
			HorizontalWorpPass = true;
			VirticalWorpPass = false;
		}
	}
	
	if (OtherPlayerReverseValue != OtherPlayer->GetReverseValue())
	{
		int Temp = OtherPlayerDownCol;
		OtherPlayerDownCol = OtherPlayerUpCol;
		OtherPlayerUpCol = Temp;
		OtherPlayerReverseValue = OtherPlayer->GetReverseValue();
	}


	// 상태에 따른 Update
	switch (CurState)
	{
	case PlayerState::Idle:
		IdleUpdate(_DeltaTime);
		break;
	case PlayerState::Run:
		RunUpdate(_DeltaTime);
		break;
	case PlayerState::Jump:
		JumpUpdate(_DeltaTime);
		break;
	case PlayerState::Fall:
		FallUpdate(_DeltaTime);
		break;
	case PlayerState::Dead:
		DeadUpdate(_DeltaTime);
		break;
	case PlayerState::Crouch:
		CrouchUpdate(_DeltaTime);
		break;
	case PlayerState::RidingMode:
		RidingModeUpdate(_DeltaTime);
		break;
	default:
		break;
	}

	std::vector<GameEngineCollision*> _Col;

	if (true == BodyCol->Collision(CollisionOrder::MonsterBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
		SetAnimation("Dead");
		ChangeState(PlayerState::Dead);
	}

	//임시 카메라 위치
	CameraFocus();
}

void Player::Render(float _DeltaTime)
{
	if (true == CheckPosOn)
	{
		CollisionData Data;
		HDC dc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
		Data.Scale = { 3,3 };
		Data.Pos = ActorCameraPos() + MapLeftUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapMiddleUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapMiddleDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftMiddleCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightMiddleCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	}
}

void Player::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;
	if (ReverseValue == false)
	{
		switch (CurDir)
		{
		case PlayerDir::Left:
			AnimationName = "Left_";
			break;
		case PlayerDir::Right:
			AnimationName = "Right_";
			break;
		default:
			break;
		}
	}
	else
	{
		switch (CurDir)
		{
		case PlayerDir::Left:
			AnimationName = "ReverseLeft_";
			break;
		case PlayerDir::Right:
			AnimationName = "ReverseRight_";
			break;
		default:
			break;
		}
	}

	AnimationName += _State;
	MainRenderer->ChangeAnimation(AnimationName, _StartFrame);
}

void Player::OtherPlayerMoveCheck()
{
	// 플레이어 Push에 인한 벽 충돌 체크
	{
		if (true == LeftToOtherRightCheck())
		{
			unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
			unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
			unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);

			if (RightUpColor == RGB(255, 0, 0) ||
				RightMiddleColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				// 벽에 박힘 제거
				while (RightUpColor == RGB(255, 0, 0) ||
					RightMiddleColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
				{
					RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::DOWN);
					RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::UP);
					AddPos(float4::LEFT);
					OtherPlayer->AddPos(float4::LEFT);
				}

				AddPos(float4::RIGHT);
				OtherPlayer->AddPos(float4::RIGHT);
			}
		}
		if (true == RightToOtherLeftCheck())
		{
			unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
			unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
			unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

			if (LeftUpColor == RGB(255, 0, 0) ||
				LeftMiddleColor == RGB(255, 0, 0) ||
				LeftDownColor == RGB(255, 0, 0))
			{
				// 벽에 박힘 제거
				while (LeftUpColor == RGB(255, 0, 0) || LeftMiddleColor == RGB(255, 0, 0) || LeftDownColor == RGB(255, 0, 0))
				{
					LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
					LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
					AddPos(float4::RIGHT);
					OtherPlayer->AddPos(float4::RIGHT);
				}
				AddPos(float4::LEFT);
				OtherPlayer->AddPos(float4::LEFT);
			}
		}
	}
}

void Player::ReverseCol()
{
	GameEngineCollision* TempCol = DownCol;
	DownCol = UpCol;
	UpCol = TempCol;
}

void Player::IdleInitFromFall()
{
	// 변환한 외부 설정 초기화
	GravityReset();
	HorizontalWorpPass = false;
	VirticalWorpPass = false;
	PrevAreaVectorInit = false;

	// 바닥에 닿을 시 워프홀 상호작용 때 추가된 중력 초기화
	if (true == ReverseValue)
	{
		SetGravityPower(-DefaultGravityPower);
	}
	else
	{
		SetGravityPower(DefaultGravityPower);
	}
	MovePos = float4::ZERO;
}

void Player::ReverseInit()
{
	unsigned int MidColor = GetGroundColor(RGB(0, 255, 255), float4::ZERO);
	if (MidColor == RGB(0, 255, 255) && false == ReverseValue)
	{
		ReverseCol();
		ReverseValue = true;

		SetGravityPower(-DefaultGravityPower);

		if (true == HorizontalWorpPass)
		{
			// 중력 한계치 초기화
			if (false == PrevAreaVectorInit)
			{
				PrevAreaVector.Y = GetGravityVector().Y;

				// 중력이 너무 작아 구멍에서 못 빠져나올 가능성 제거
				if (PrevAreaVector.Y < MinGravityInHole)
				{
					PrevAreaVector.Y = MinGravityInHole;
				}

				PrevAreaVectorInit = true;
			}

			// 영역반전 시 중력의 변화량이 중력 오차범위를 넘었다면 "중력 기준치" 변경
			if (GetGravityVector().Y > PrevAreaVector.Y * ErrorRangeOfGravity)
			{
				PrevAreaVector.Y = -GetGravityVector().Y;

				// 레벨기준 좌표 이상으로 올라갈 가능성 제거
				if (PrevAreaVector.Y < -MaxGravityInHole)
				{
					PrevAreaVector.Y = -MaxGravityInHole;
				}

				// Vector를 이전 중력값으로 변경하기 전에 PrevAreaVector를 음수화 하였으므로 음수로 넣어줌
				SetGravityVector(-PrevAreaVector);
			}
			else
			{
				// 레벨기준 좌표 이상으로 올라갈 가능성 제거
				if (PrevAreaVector.Y > MaxGravityInHole)
				{
					PrevAreaVector.Y = MaxGravityInHole;
				}

				// 오차범위 안이라면 DeltaTime로 인해서 생긴 GravityVector 오차 제거
				SetGravityVector(PrevAreaVector);
				PrevAreaVector.Y = -GetGravityVector().Y;
			}
		}
	}
	else if (MidColor == RGB(255, 255, 255) && true == ReverseValue)
	{
		ReverseCol();
		ReverseValue = false;
		SetGravityPower(DefaultGravityPower);

		if (true == HorizontalWorpPass)
		{
			// 초기화
			if (false == PrevAreaVectorInit)
			{
				PrevAreaVector.Y = GetGravityVector().Y;

				// 중력이 너무 작아 구멍에서 못 빠져나올 가능성 제거
				if (PrevAreaVector.Y > -MinGravityInHole)
				{
					PrevAreaVector.Y = -MinGravityInHole;
				}

				PrevAreaVectorInit = true;
			}

			// 영역반전 시 중력의 변화량이 중력 오차범위를 넘었다면 "중력 기준치" 변경
			if (GetGravityVector().Y < PrevAreaVector.Y * ErrorRangeOfGravity)
			{
				PrevAreaVector.Y = -GetGravityVector().Y;

				// 레벨기준 좌표 이상으로 올라갈 가능성 제거
				if (PrevAreaVector.Y > MaxGravityInHole)
				{
					PrevAreaVector.Y = MaxGravityInHole;
				}

				SetGravityVector(-PrevAreaVector);
			}
			else
			{
				// 레벨기준 좌표 이상으로 올라갈 가능성 제거
				if (PrevAreaVector.Y < -MaxGravityInHole)
				{
					PrevAreaVector.Y = -MaxGravityInHole;
				}

				// 오차범위 안이라면 DeltaTime로 인해서 생긴 GravityVector 오차 제거
				SetGravityVector(PrevAreaVector);
				PrevAreaVector.Y = -GetGravityVector().Y;
			}
		}
	}
}


bool Player::LeftMapColCheck()
{
	// 좌측 충돌 체크
	unsigned int LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::LEFT + float4::DOWN);
	unsigned int LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck + float4::LEFT);
	unsigned int LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::LEFT + float4::UP);

	if (LeftUpColor == RGB(255, 0, 0) ||
		LeftMiddleColor == RGB(255, 0, 0) ||
		LeftDownColor == RGB(255, 0, 0))
	{
		if (CurState == PlayerState::Run ||
			CurState == PlayerState::RidingMode)
		{
			// 벽에 박힘 제거
			while (LeftUpColor == RGB(255, 0, 0) ||
				LeftMiddleColor == RGB(255, 0, 0) ||
				LeftDownColor == RGB(255, 0, 0))
			{
				LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::DOWN);
				LeftMiddleColor = GetGroundColor(RGB(255, 0, 0), MapLeftMiddleCheck);
				LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::UP);
				AddPos(float4::RIGHT);
			}
			AddPos(float4::LEFT);
		}

		MovePos = float4::ZERO;
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::RightMapColCheck()
{
	// 우측 충돌 체크
	unsigned int RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::RIGHT + float4::DOWN);
	unsigned int RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck + float4::RIGHT);
	unsigned int RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::RIGHT + float4::UP);

	if (RightUpColor == RGB(255, 0, 0) ||
		RightMiddleColor == RGB(255, 0, 0) ||
		RightDownColor == RGB(255, 0, 0))
	{
		if (CurState == PlayerState::Run ||
			CurState == PlayerState::RidingMode)
		{
			// 벽에 박힘 제거
			while (RightUpColor == RGB(255, 0, 0) ||
				RightMiddleColor == RGB(255, 0, 0) ||
				RightDownColor == RGB(255, 0, 0))
			{
				RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::DOWN);
				RightMiddleColor = GetGroundColor(RGB(255, 0, 0), MapRightMiddleCheck);
				RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::UP);
				AddPos(float4::LEFT);
			}
			AddPos(float4::RIGHT);
		}

		MovePos = float4::ZERO;
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::UpMapColCheck()
{
	unsigned int LeftUpColor;
	unsigned int MiddleUpColor;
	unsigned int RightUpColor;

	if (true == ReverseValue)
	{
		LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
		MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
		RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);

		if (LeftUpColor == RGB(255, 0, 0) ||
			MiddleUpColor == RGB(255, 0, 0) ||
			RightUpColor == RGB(255, 0, 0))
		{
			while (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck - float4::LEFT);
				MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
				RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck - float4::RIGHT);
				AddPos(float4::UP);
			}
			return true;
		}
		else false;
	}
	else
	{
		LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
		MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
		RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
		if (LeftUpColor == RGB(255, 0, 0) ||
			MiddleUpColor == RGB(255, 0, 0) ||
			RightUpColor == RGB(255, 0, 0))
		{
			while (LeftUpColor == RGB(255, 0, 0) ||
				MiddleUpColor == RGB(255, 0, 0) ||
				RightUpColor == RGB(255, 0, 0))
			{
				LeftUpColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck - float4::LEFT);
				MiddleUpColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
				RightUpColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck - float4::RIGHT);
				AddPos(float4::DOWN);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Player::DownMapColCheck()
{
	unsigned int LeftDownColor;
	unsigned int MiddleDownColor;
	unsigned int RightDownColor;

	if (true == ReverseValue)
	{
		LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::UP + float4::RIGHT * 2.0f);
		MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck + float4::UP);
		RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::UP + float4::LEFT * 2.0f);

		if (LeftDownColor == RGB(255, 0, 0) ||
			MiddleDownColor == RGB(255, 0, 0) ||
			RightDownColor == RGB(255, 0, 0))
		{
			if (PlayerState::Fall == CurState)
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftUpCheck + float4::RIGHT * 2.0f);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleUpCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightUpCheck + float4::LEFT * 2.0f);

					AddPos(float4::DOWN);
				}
				// 지글현상 제거
				AddPos(float4::UP);
			}
			return true;
		}
	}
	else
	{
		LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::DOWN + float4::RIGHT * 2.0f);
		MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck + float4::DOWN);
		RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::DOWN + float4::LEFT * 2.0f);

		if (LeftDownColor == RGB(255, 0, 0) ||
			MiddleDownColor == RGB(255, 0, 0) ||
			RightDownColor == RGB(255, 0, 0))
		{

			if (PlayerState::Fall == CurState)
			{
				while (LeftDownColor == RGB(255, 0, 0) ||
					MiddleDownColor == RGB(255, 0, 0) ||
					RightDownColor == RGB(255, 0, 0))
				{
					LeftDownColor = GetGroundColor(RGB(255, 0, 0), MapLeftDownCheck + float4::RIGHT * 2.0f);
					MiddleDownColor = GetGroundColor(RGB(255, 0, 0), MapMiddleDownCheck);
					RightDownColor = GetGroundColor(RGB(255, 0, 0), MapRightDownCheck + float4::LEFT * 2.0f);

					AddPos(float4::UP);
				}
				// 지글현상 제거
				AddPos(float4::DOWN);
			}
			return true;
		}
	}

	return false;
}
