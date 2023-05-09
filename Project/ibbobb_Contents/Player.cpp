#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/ResourceManager.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

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
		FilePath.MoveChild("Resource\\Texture\\Image.Bmp");
		ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());
	}

	SetPos({ 200, 200 });
	SetScale({ 50, 0 });

	//SetPos({ 200, 200 });
	//SetScale({ 100, 100 });
	
}

void Player::Update(float _Delta)
{
	//static float a = 0.0f;
	//if (a < 100)
	//{
	//	AddPos({ 1.0f, 0.0f });

	//	a++;
	//}
	
}

void Player::Render()
{
	HDC WindowDC = GameEngineWindow::MainWindow.GetHDC();
	GameEngineWindowTexture* FindTexture = ResourceManager::GetInst().FindTexture("Image.Bmp");
	HDC ImageDC = FindTexture->GetImageDC();

	BitBlt(WindowDC,
	    GetPos().iX() - GetScale().ihX(),
		GetPos().iY() - GetScale().ihY(),
		GetPos().iX() + GetScale().ihX(),
		GetPos().iY() + GetScale().ihY(),
		ImageDC, 0, 0, SRCCOPY);

}

void Player::Release()
{

}