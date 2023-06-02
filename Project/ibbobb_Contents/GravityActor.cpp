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

	AddPos(GravityVector);
}

void GravityActor::CameraFocus()
{
	float4 WindowScale = GameEngineWindow::MainWindow.GetScale();
	float4 Value = float4{ -WindowScale.hX(), -WindowScale.hY()};
	GetLevel()->GetMainCamera()->SetPos(GetPos() + Value);
}

void GravityActor::SetGroundTexture(const std::string& _GroundTextureName)
{
	GroundTexture = ResourcesManager::GetInst().FindTexture(_GroundTextureName);

	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� �ȼ��浹�� �Ϸ��� �߽��ϴ�.");
	}

}

int GravityActor::GetGroundColor(unsigned int _DefaultColor, float4 _Pos)
{
	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("�� üũ�� �ؽ�ó�� �������� �ʽ��ϴ�.");
	}

	return GroundTexture->GetColor(_DefaultColor, GetPos() + _Pos);
}

float4 GravityActor::ActorCameraPos()
{
	return GetPos() - GetLevel()->GetMainCamera()->GetPos();
}