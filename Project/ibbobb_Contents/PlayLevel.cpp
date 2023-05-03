#include "PlayLevel.h"
#include "Player.h"
PlayLevel::PlayLevel()
{

}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start() 
{
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