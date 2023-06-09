#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"

#include <windows.h>

Player::Player()
{

}

Player::~Player()
{

}

void Player::Start()
{
	GameEnginePath FilePath;
	FilePath.SetCurrentPath();
	FilePath.MoveParentToExistsChild("Resources");
	FilePath.MoveChild("Resources\\Texture\\Characters\\obb\\");

	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Left_obb.bmp"), 7, 6);
	ResourcesManager::GetInst().CreateSpriteSheet(FilePath.PlusFilePath("Right_obb.bmp"), 7, 6);

	MainRenderer = CreateRenderer(RenderOrder::Player);

	// UP ���� �ִϸ��̼�
	{
		// Idle Animation
		MainRenderer->CreateAnimation("UpLeft_Idle", "Left_obb.bmp", 0, 0, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Idle", "Right_obb.bmp", 0, 0, 10.0f, true);

		// Blink Animation
		MainRenderer->CreateAnimation("UpLeft_Blink", "Left_obb.bmp", 0, 1, 0.07f, true);
		MainRenderer->CreateAnimation("UpRight_Blink", "Right_obb.bmp", 0, 1, 0.07f, true);

		// Run Animation
		MainRenderer->CreateAnimation("UpLeft_Run", "Left_obb.bmp", 12, 21, 0.08f, true);
		MainRenderer->CreateAnimation("UpRight_Run", "Right_obb.bmp", 12, 21, 0.08f, true);

		// Jump Animation
		MainRenderer->CreateAnimation("UpLeft_Jump", "Left_obb.bmp", 22, 22, 10.0f, true);
		MainRenderer->CreateAnimation("UpRight_Jump", "Right_obb.bmp", 22, 22, 10.0f, true);

		// Tumbling Animation
		MainRenderer->CreateAnimation("UpLeft_Tumbling", "Left_obb.bmp", 23, 28, 0.05f, true);
		MainRenderer->CreateAnimation("UpRight_Tumbling", "Right_obb.bmp", 23, 28, 0.05f, true);

		// Fall Animation
		MainRenderer->CreateAnimation("UpLeft_Fall", "Left_obb.bmp", 29, 30, 0.07f, true);
		MainRenderer->CreateAnimation("UpRight_Fall", "Right_obb.bmp", 29, 30, 0.07f, true);

		// Dead Animation
		MainRenderer->CreateAnimation("UpLeft_Dead", "Left_obb.bmp", 35, 41, 0.03f, true);
		MainRenderer->CreateAnimation("UpRight_Dead", "Right_obb.bmp", 35, 41, 0.03f, true);
	}

	// ù ���� ����
	ChangeState(PlayerState::Fall);

	// ù ���� ����
	CurDir = PlayerDir::UpRight;

	// �̰� ���� �Լ�?
	//MainRenderer->SetRenderScaleToTexture();

	Ratio = 2.0f;
	MainRenderer->SetScaleRatio(Ratio);

	// �߷°� ����
	SetGravityPower(3.0f);

	// ������ ����
	SetJumpForce(1.2f);

	// �ӵ� ����
	SetSpeed(80.0f);

	// ��ġ ����
	SetPos({ 300.0f, 600.0f });
}

void Player::Update(float _DeltaTime)
{
	
	// ���� �ܾƿ� 
	if (true == GameEngineInput::IsPress('Z'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(0.005f);
	}

	if (true == GameEngineInput::IsPress('X'))
	{
		GameEngineWindow::MainWindow.AddDoubleBufferingCopyScaleRatio(-0.005f);
		float ZoomScale = GameEngineWindow::MainWindow.GetDoubleBufferingCopyScaleRatio();
		if (1.0f > ZoomScale)
		{
			GameEngineWindow::MainWindow.SetDoubleBufferingCopyScaleRatio(1.0f);
		}
	}

	//if (true == MainRenderer->IsAnimationEnd())
	//{
	//	Death();
	//}

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
	default:
		break;
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
		Data.Scale = { 5,5 };
		Data.Pos = ActorCameraPos() + MapLeftUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightUpCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapUpCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightDownCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapDownCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());

		Data.Pos = ActorCameraPos() + MapLeftCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
		Data.Pos = ActorCameraPos() + MapRightCenterCheck;
		Rectangle(dc, Data.iLeft(), Data.iTop(), Data.iRight(), Data.iBot());
	}
}

void Player::SetAnimation(const std::string _State, int _StartFrame)
{
	std::string AnimationName;

	switch (CurDir)
	{
	case PlayerDir::UpLeft:
		AnimationName = "UpLeft_";
		break;
	case PlayerDir::UpRight:
		AnimationName = "UpRight_";
		break;
	case PlayerDir::DownLeft:
		AnimationName = "DownLeft_";
		break;
	case PlayerDir::DownRight:
		AnimationName = "DownRight_";
		break;
	default:
		break;
	}

	AnimationName += _State;
	MainRenderer->ChangeAnimation(AnimationName, _StartFrame);
}


void Player::ChangeState(PlayerState _State)
{
	State = _State;
}
