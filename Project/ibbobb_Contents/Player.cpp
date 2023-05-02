#include "Player.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEngineTime.h>

Player::Player()
{
	
}

Player::~Player()
{

}

void Player::Start()
{
	SetPos({ 200, 200 });
	SetScale({ 100, 100 });
}

void Player::Update()
{
	GameEngineTime::MainTimer;
	float Time = GameEngineTime::MainTimer.GetDeltaTime();
	AddPos({ 0.001f, 0.0f });
}

void Player::Render()
{
	HDC WindowDC = GameEngineWindow::MainWindow.GetHDC();
	Rectangle(WindowDC,
		GetPos().iX() - GetScale().ihX(),
		GetPos().iY() - GetScale().ihY(),
		GetPos().iX() + GetScale().ihX(),
		GetPos().iY() + GetScale().ihY()
	);
}

void Player::Release()
{

}