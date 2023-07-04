#include "Player.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <string>

#include "SoundLoadManager.h"
void Player::PlayerEffectSoundLoad()
{
	if (nullptr == GameEngineSound::FindSound("PlayerJump.mp3"))
	{
		SoundLoadManager::LoadSound("PlayerState", "PlayerJump.mp3");
		
	}

	if (nullptr == GameEngineSound::FindSound("PlayerCrouch.mp3"))
	{
		SoundLoadManager::LoadSound("PlayerState", "PlayerCrouch.mp3");
	}

	for (int i = 1; i <= 10; i++)
	{
		std::string _Name = "WarpPass";
		_Name += std::to_string(i);
		_Name += ".mp3";
		if (nullptr == GameEngineSound::FindSound(_Name))
		{
			SoundLoadManager::LoadSound("Warp", _Name);
		}
	}

	for (int i = 1; i <= 10; i++)
	{
		std::string _Name = "LongWarpPass";
		_Name += std::to_string(i);
		_Name += ".mp3";
		if (nullptr == GameEngineSound::FindSound(_Name))
		{
			SoundLoadManager::LoadSound("Warp", _Name);
		}
	}

}
void Player::JumpSoundPlay()
{
	EffectPlayer = GameEngineSound::SoundPlay("PlayerJump.mp3");
	EffectPlayer.SetVolume(0.07f);
}

void Player::CrouchSoundPlay()
{
	EffectPlayer = GameEngineSound::SoundPlay("PlayerCrouch.mp3");
	EffectPlayer.SetVolume(0.3f);
}

void Player::DeadSoundPlay()
{

}

void Player::WarpPassSoundPlay()
{
	int _Num = GameEngineRandom::MainRandom.RandomInt(1, 10);
	switch (_Num)
	{
	case 1:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass1.mp3");
		break;
	case 2:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass2.mp3");
		break;
	case 3:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass3.mp3");
		break;
	case 4:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass4.mp3");
		break;
	case 5:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass5.mp3");
		break;
	case 6:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass6.mp3");
		break;
	case 7:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass7.mp3");
		break;
	case 8:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass8.mp3");
		break;
	case 9:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass9.mp3");
		break;
	case 10:
		EffectPlayer = GameEngineSound::SoundPlay("WarpPass10.mp3");
		break;
	default:
		break;
	}

	EffectPlayer.SetVolume(1.0f);
}

void Player::LongWarpPassSoundPlay()
{
	int _Num = GameEngineRandom::MainRandom.RandomInt(1, 10);
	switch (_Num)
	{
	case 1:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass1.mp3");
		break;
	case 2:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass2.mp3");
		break;
	case 3:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass3.mp3");
		break;
	case 4:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass4.mp3");
		break;
	case 5:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass5.mp3");
		break;
	case 6:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass6.mp3");
		break;
	case 7:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass7.mp3");
		break;
	case 8:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass8.mp3");
		break;
	case 9:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass9.mp3");
		break;
	case 10:
		EffectPlayer = GameEngineSound::SoundPlay("LongWarpPass10.mp3");
		break;
	default:
		break;
	}

	EffectPlayer.SetVolume(0.6f);
}