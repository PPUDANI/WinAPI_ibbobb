#include "PlayLevel.h"

// Contents
#include "Player.h"
#include "BackGround.h"

PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start() 
{
	BackGround* Back = CreateActor<BackGround>();
	Back->Init("Sky.Bmp");

	BackGround* Map = CreateActor<BackGround>();
	Map->Init("StageTest.Bmp");

	CreateActor<Player>();
}

void PlayLevel::Update(float _Delta)
{

}

void PlayLevel::Render() 
{

}

void PlayLevel::Release() 
{

}