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
	
	Ratio = 2.0f;

	// �߷°� ����
	DefaultGravityPower = 1200.0f;
	SetGravityPower(DefaultGravityPower);

	// ����Ȧ �⺻ �߷°� ���� (�߷°��� �۾� �� �������� ���ɼ� ����)
	HoleDefaultGravityValue = 400.0f;

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

	// ���¿� ���� Update
	switch (State)
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
	default:
		break;
	}

	std::vector<GameEngineCollision*> _Col;

	if (true == BodyCollision->Collision(CollisionOrder::MonsterBody,
		_Col,
		CollisionType::CirCle,
		CollisionType::CirCle))
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


