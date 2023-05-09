#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/ResourceManager.h>

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
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Image.Bmp");
		ResourceManager::GetInst().TextureLoad(FilePath.GetStringPath());
	}

	CreateRenderer("Image.Bmp");

	SetPos({ 200, 200 });
	SetScale({ 100, 100 });

	//SetPos({ 200, 200 });
	//SetScale({ 100, 100 });
	
}

void Player::Update(float _Delta)
{
	AddPos({ 100.0f * _Delta, 0.0f });
}

void Player::Render()
{
	GameEngineWindowTexture* BackBuffer = GameEngineWindow::MainWindow.GetBackBuffer();
	GameEngineWindowTexture* FindTexture = ResourceManager::GetInst().FindTexture("Image.Bmp");
	BackBuffer->TransCopy(FindTexture, GetPos(), { 100, 100 }, { 0, 0 }, FindTexture->GetScale());
}

void Player::Release()
{

}