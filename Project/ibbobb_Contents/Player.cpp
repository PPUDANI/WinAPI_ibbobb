#include "Player.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
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

	if (false == ResourceManager::GetInst().IsLoadTexture("obb.bmp"))
	{
		GameEnginePath FilePath;

		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Texture\\Character");
		ResourceManager::GetInst().TextureLoad(FilePath.PlusFilePath("obb.bmp"));
	}

	GameEngineRenderer* Ptr = CreateRenderer("obb.bmp", RenderOrder::Play);
	Ptr->SetRenderPos({ 0, 0 });
	Ptr->SetRenderScale({ 100, 100 });


	float4 WinScale = GameEngineWindow::MainWindow.GetScale();

	SetPos(WinScale.Half());
	
}

void Player::Update(float _Delta)
{
	float Speed = 200.0f;

	float4 MovePos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = { -Speed * _Delta, 0.0f };
	}

	if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = { Speed * _Delta, 0.0f };
	}

	if (true == GameEngineInput::IsPress('W'))
	{
		MovePos = { 0.0f, -Speed * _Delta };
	}

	if (true == GameEngineInput::IsPress('S'))
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