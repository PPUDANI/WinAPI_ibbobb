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

	// ù ���� ����
	CurDir = PlayerDir::Right;

	// �̰� ���� �Լ�?
	//MainRenderer->SetRenderScaleToTexture();
	MainRenderer->SetScaleRatio(Ratio);

	// �߷°� ����
	DefaultGravityPower = 1200.0f;
	SetGravityPower(DefaultGravityPower);

	// ����Ȧ �⺻ �߷°� ���� (�߷°��� �۾� �� �������� ���ɼ� ����)
	MinGravityInHole = 350.0f;
	MaxGravityInHole = 1100.0f;
	ErrorRangeOfGravity = 1.1f;
	PushPowerInVirticalHole = 50.0f;

	// ������ ����
	JumpForce = 550.0f;

	// �ӵ� ����
	Speed = 200.0f;

}

void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		CheckPosOn = !CheckPosOn;
	}

	// ���ο���, ���ο��� ����
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


	// ���¿� ���� Update
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

	//�ӽ� ī�޶� ��ġ
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
	// �÷��̾� Push�� ���� �� �浹 üũ
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
				// ���� ���� ����
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
				// ���� ���� ����
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
	// ��ȯ�� �ܺ� ���� �ʱ�ȭ
	GravityReset();
	HorizontalWorpPass = false;
	VirticalWorpPass = false;
	PrevAreaVectorInit = false;

	// �ٴڿ� ���� �� ����Ȧ ��ȣ�ۿ� �� �߰��� �߷� �ʱ�ȭ
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
			// �߷� �Ѱ�ġ �ʱ�ȭ
			if (false == PrevAreaVectorInit)
			{
				PrevAreaVector.Y = GetGravityVector().Y;

				// �߷��� �ʹ� �۾� ���ۿ��� �� �������� ���ɼ� ����
				if (PrevAreaVector.Y < MinGravityInHole)
				{
					PrevAreaVector.Y = MinGravityInHole;
				}

				PrevAreaVectorInit = true;
			}

			// �������� �� �߷��� ��ȭ���� �߷� ���������� �Ѿ��ٸ� "�߷� ����ġ" ����
			if (GetGravityVector().Y > PrevAreaVector.Y * ErrorRangeOfGravity)
			{
				PrevAreaVector.Y = -GetGravityVector().Y;

				// �������� ��ǥ �̻����� �ö� ���ɼ� ����
				if (PrevAreaVector.Y < -MaxGravityInHole)
				{
					PrevAreaVector.Y = -MaxGravityInHole;
				}

				// Vector�� ���� �߷°����� �����ϱ� ���� PrevAreaVector�� ����ȭ �Ͽ����Ƿ� ������ �־���
				SetGravityVector(-PrevAreaVector);
			}
			else
			{
				// �������� ��ǥ �̻����� �ö� ���ɼ� ����
				if (PrevAreaVector.Y > MaxGravityInHole)
				{
					PrevAreaVector.Y = MaxGravityInHole;
				}

				// �������� ���̶�� DeltaTime�� ���ؼ� ���� GravityVector ���� ����
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
			// �ʱ�ȭ
			if (false == PrevAreaVectorInit)
			{
				PrevAreaVector.Y = GetGravityVector().Y;

				// �߷��� �ʹ� �۾� ���ۿ��� �� �������� ���ɼ� ����
				if (PrevAreaVector.Y > -MinGravityInHole)
				{
					PrevAreaVector.Y = -MinGravityInHole;
				}

				PrevAreaVectorInit = true;
			}

			// �������� �� �߷��� ��ȭ���� �߷� ���������� �Ѿ��ٸ� "�߷� ����ġ" ����
			if (GetGravityVector().Y < PrevAreaVector.Y * ErrorRangeOfGravity)
			{
				PrevAreaVector.Y = -GetGravityVector().Y;

				// �������� ��ǥ �̻����� �ö� ���ɼ� ����
				if (PrevAreaVector.Y > MaxGravityInHole)
				{
					PrevAreaVector.Y = MaxGravityInHole;
				}

				SetGravityVector(-PrevAreaVector);
			}
			else
			{
				// �������� ��ǥ �̻����� �ö� ���ɼ� ����
				if (PrevAreaVector.Y < -MaxGravityInHole)
				{
					PrevAreaVector.Y = -MaxGravityInHole;
				}

				// �������� ���̶�� DeltaTime�� ���ؼ� ���� GravityVector ���� ����
				SetGravityVector(PrevAreaVector);
				PrevAreaVector.Y = -GetGravityVector().Y;
			}
		}
	}
}


bool Player::LeftMapColCheck()
{
	// ���� �浹 üũ
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
			// ���� ���� ����
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
	// ���� �浹 üũ
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
			// ���� ���� ����
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
				// �������� ����
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
				// �������� ����
				AddPos(float4::DOWN);
			}
			return true;
		}
	}

	return false;
}
