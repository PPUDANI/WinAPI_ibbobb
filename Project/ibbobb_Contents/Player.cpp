#include "Player.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineCore/ResourceManager.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineTexture.h>

#include <windows.h>

Player::Player()
{
	
}

Player::~Player()
{

}

void Player::Start()
{

	if (false == ResourceManager::GetInst().IsLoadTexture("Image.Bmp"))
	{
		GameEnginePath FilePath;

		FilePath.GetCurrentPath();
		FilePath.MoveParentToExistsChild("Resource");
		FilePath.MoveChiled("Resource\\Texture\\Image.Bmp");
		ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());
	}
	SetPos({ 200, 200 });
	SetScale({ 100, 100 });
}

void Player::Update(float _Delta)
{
	AddPos({ 100.0f * _Delta, 0.0f });
}

void Player::Render()
{
	HDC WindowDC = GameEngineWindow::MainWindow.GetHDC();
	GameEngineTexture* FindTexture = ResourceManager::GetInst().FindTexture("Image.Bmp");
	HDC ImageDC = FindTexture->GetImageDC();

	BitBlt(WindowDC, 100, 100, 500, 500, ImageDC, 0, 0, SRCCOPY);

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