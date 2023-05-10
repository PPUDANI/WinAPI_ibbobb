#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/ResourceManager.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
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

	if (false == ResourceManager::GetInst().IsLoadTexture("Test.Bmp"))
	{
		GameEnginePath FilePath;

		FilePath.GetCurrentPath();
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Character");
		ResourceManager::GetInst().TextureLoad(FilePath.PlusFilePath("Test.bmp"));
		ResourceManager::GetInst().TextureLoad(FilePath.PlusFilePath("HPBar.bmp"));
	}


	{
		GameEngineRenderer* Ptr = CreateRenderer("Test.Bmp", RenderOrder::Play);
		Ptr->SetRenderScale({ 200, 200 });
		Ptr->SetTexture("Test.Bmp");
	}

	{
		GameEngineRenderer* Ptr = CreateRenderer("HPBar.bmp", RenderOrder::Play);
		Ptr->SetRenderPos({ 0, -100 });
		Ptr->SetRenderScale({ 200, 40 });
		Ptr->SetTexture("HPBar.bmp");
	}

	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	SetPos(WinScale.Half());
	
}

void Player::Update(float _Delta)
{
	float Speed = 200.0f;

	float4 MovePos = float4::ZERO;

	if (0 != GetAsyncKeyState('A'))
	{
		MovePos = { -Speed * _Delta, 0.0f };
	}

	if (0 != GetAsyncKeyState('D'))
	{
		MovePos = { Speed * _Delta, 0.0f };
	}

	if (0 != GetAsyncKeyState('W'))
	{
		MovePos = { 0.0f, -Speed * _Delta };
	}

	if (0 != GetAsyncKeyState('S'))
	{
		MovePos = { 0.0f, Speed * _Delta };
	}

	AddPos(MovePos);
	GetLevel()->GetMainCamera()->AddPos(MovePos);
}

void Player::Render()
{
	//GameEngineWindowTexture* BackBuffer = GameEngineWindow::MainWindow.GetBackBuffer();
	//GameEngineWindowTexture* FindTexture = ResourceManager::GetInst().FindTexture("Test.Bmp");
	//BackBuffer->TransCopy(FindTexture, GetPos(), { 100, 100 }, { 0, 0 }, FindTexture->GetScale());
}

void Player::Release()
{

}