#include "SoundLoadManager.h"
#include <GameEnginePlatform/GameEngineSound.h>

SoundLoadManager::SoundLoadManager()
{
}

SoundLoadManager::~SoundLoadManager()
{
}

void SoundLoadManager::LoadSound(const std::string _TypeName, const std::string _FileName)
{
	if (nullptr == GameEngineSound::FindSound(_FileName))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("Resources");
		FilePath.MoveChild("Resources\\Sound\\" + _TypeName + "\\");

		GameEngineSound::SoundLoad(FilePath.PlusFilePath(_FileName));
	}
}
