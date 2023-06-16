#include "GravityActor.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/ResourcesManager.h>

GravityActor::GravityActor()
{

}

GravityActor::~GravityActor()
{

}

void GravityActor::Gravity(float _DeltaTime)
{
	if (false == GravityValue)
	{
		return;
	}

	GravityVector += float4::DOWN * GravityPower * _DeltaTime;

	float AbsoluteVector = GravityVector.Y;
	if (GravityLimit < AbsoluteVector)
	{
		GravityVector.Y = GravityLimit;
	}
	else if (-GravityLimit > AbsoluteVector)
	{
		GravityVector.Y = -GravityLimit;
	}

	AddPos(GravityVector * _DeltaTime);
}

void GravityActor::CameraFocus()
{
	float4 WindowScale = GameEngineWindow::MainWindow.GetScale();

	float4 Value = float4{ -WindowScale.hX(), -WindowScale.hY()};

	float4 PlayerPos = GetPos();

	//if (PlayerPos.X - WindowScale.hX() <= 0.0f)
	//{
	//	PlayerPos.X = WindowScale.hX();
	//}

	GetLevel()->GetMainCamera()->SetPos(PlayerPos + Value);
}

void GravityActor::SetGroundTexture(const std::string& _GroundTextureName)
{
	GroundTexture = ResourcesManager::GetInst().FindTexture(_GroundTextureName);

	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("존재하지 않는 텍스처로 픽셀충돌을 하려고 했습니다.");
	}

}

int GravityActor::GetGroundColor(unsigned int _DefaultColor, float4 _Pos)
{
	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("땅 체크용 텍스처가 존재하지 않습니다.");
	}

	return GroundTexture->GetColor(_DefaultColor, GetPos() + _Pos);
}

float4 GravityActor::ActorCameraPos()
{
	return GetPos() - GetLevel()->GetMainCamera()->GetPos();
}