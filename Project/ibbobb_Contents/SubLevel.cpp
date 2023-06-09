#include "SubLevel.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/ResourcesManager.h>
#include <cmath>

#include "ContentsEnum.h"
#include "BackGround.h"
#include "Map.h"
#include "ibb.h"
#include "obb.h"

bool SubLevel::DevelopmentMode = false;

SubLevel::SubLevel()
{
	WindowScale = GameEngineWindow::MainWindow.GetScale();
}

SubLevel::~SubLevel()
{
}

void SubLevel::Start()
{

}

void SubLevel::Update(float _DeltaTime)
{
	if (ibbPlayer->GetOtherPlayer() == nullptr ||
		obbPlayer->GetOtherPlayer() == nullptr)
	{
		MsgBoxAssert("OtherPlayer가 세팅되지 않았습니다.")
	}

	if (true == GameEngineInput::IsDown('I') && true == DevelopmentMode)
	{
		LevelMap->SwitchRender();
		CollisionDebugRenderSwitch();
	}

	float4 ibbPlayerPos = ibbPlayer->GetPos();
	float4 obbPlayerPos = obbPlayer->GetPos();
	float4 VectorFromibbToobb = obbPlayerPos - ibbPlayerPos;
	float DistanceXFromibbToobb = obbPlayerPos.X - ibbPlayerPos.X;
	float4 CameraPos;

	if (false == ibbPlayer->IsUpdate() &&
		false == obbPlayer->IsUpdate())
	{
		if (false == FirstDeathIsibb)
		{
			CameraPos = ibbPlayer->GetPos() - WindowScale.Half();
		}
		else
		{
			CameraPos = obbPlayer->GetPos() - WindowScale.Half();
		}
	}
	else if (false == ibbPlayer->IsUpdate())
	{
		FirstDeathIsibb = true;
		CameraPos = obbPlayer->GetPos() - WindowScale.Half();
	}
	else if (false == obbPlayer->IsUpdate())
	{
		FirstDeathIsibb = false;
		CameraPos = ibbPlayer->GetPos() - WindowScale.Half();
	}
	else
	{
		CameraPos = (ibbPlayerPos + (VectorFromibbToobb) * 0.5f - WindowScale.Half());
	}

	if (LevelMaxScaleX == 0.0f)
	{
		MsgBoxAssert("LevelMaxScaleX 값을 초기화 하지 않았습니다.");
	}

	float4 WindowHalfX = WindowScale.Half();
	if (CameraPos.X < 0.0f)
	{
		CameraPos.X = 0.0f;
	}
	else if (CameraPos.X > LevelMaxScaleX - WindowScale.X)
	{
		CameraPos.X = LevelMaxScaleX - WindowScale.X;
	}

	GetMainCamera()->SetPos(CameraPos);
	Back->SetPos(GetMainCamera()->GetPos() + WindowScale.Half());
}

void SubLevel::ReviveCharacter()
{
	ibbPlayer->On();
	obbPlayer->On();

	ibbPlayer->GravityReset();
	obbPlayer->GravityReset();

	ibbPlayer->ChangeState(PlayerState::Fall);
	obbPlayer->ChangeState(PlayerState::Fall);
	ZoomScale = 1.0f;
}
