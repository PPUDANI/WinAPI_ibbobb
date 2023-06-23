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
		unsigned int LeftColor = GetGroundColor(RGB(0, 255, 255), MapLeftMiddleCheck);
		unsigned int RightColor = GetGroundColor(RGB(0, 255, 255), MapRightMiddleCheck);
		unsigned int UpColor = GetGroundColor(RGB(0, 255, 255), MapMiddleUpCheck);
		unsigned int DownColor = GetGroundColor(RGB(0, 255, 255), MapMiddleDownCheck);
		if (LeftColor != RightColor)
		{
			LengthWorpPass = false;
			WidthWorpPass = true;
		}
		else if (UpColor != DownColor)
		{
			LengthWorpPass = true;
			WidthWorpPass = false;
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
	case PlayerState::RidingMode:
		RidingModeUpdate(_DeltaTime);
	default:
		break;
	}

	std::vector<GameEngineCollision*> _Col;

	if (true == BodyCol->Collision(CollisionOrder::MonsterBody,
		_Col,
		CollisionType::Rect,
		CollisionType::Rect))
	{
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
