#include "ResourceManager.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

ResourceManager ResourceManager::Inst;

ResourceManager::ResourceManager() 
{
}

ResourceManager::~ResourceManager() 
{
	for (const std::pair<std::string, GameEngineWindowTexture*>& Pair : AllTexture)
	{
		GameEngineWindowTexture* Texture = Pair.second;

		if (nullptr != Texture)
		{
			delete Texture;
			Texture = nullptr;
		}
	}
}

bool ResourceManager::IsLoadTexture(const std::string& _Name)
{
	return FindTexture(_Name) != nullptr;
}

GameEngineWindowTexture* ResourceManager::FindTexture(const std::string& _Name)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	std::map<std::string, GameEngineWindowTexture*>::iterator FindIter = AllTexture.find(UpperName);

	if (FindIter == AllTexture.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

GameEngineWindowTexture* ResourceManager::TextureLoad(const std::string _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	GameEngineWindowTexture* LoadTexture = new GameEngineWindowTexture();

	LoadTexture->ResLoad(_Path);

	AllTexture.insert(std::make_pair(UpperName, LoadTexture));

	return LoadTexture;
}