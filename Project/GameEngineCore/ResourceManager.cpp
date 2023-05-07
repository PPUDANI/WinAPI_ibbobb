#include "ResourceManager.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineCore/GameEngineTexture.h>

ResourceManager ResourceManager::Inst;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() 
{
	for (const std::pair<std::string, GameEngineTexture*>& Pair : AllTexture)
	{
		GameEngineTexture* Texture = Pair.second;

		if (nullptr != Texture)
		{
			delete Texture;
			Texture = nullptr;
		}
	}
}

GameEngineTexture* ResourceManager::FindTexture(const std::string& _Name)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	std::map<std::string, GameEngineTexture*>::iterator FindIter = AllTexture.find(UpperName);

	if (FindIter == AllTexture.end())
	{
		return nullptr;
	}

	return FindIter->second;
}


bool ResourceManager::IsLoadTexture(const std::string& _Name)
{
	return FindTexture(_Name) != nullptr;
}

void ResourceManager::TextureLoad(const std::string _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	GameEngineTexture* LoadTexture = new GameEngineTexture();

	LoadTexture->ResLoad(_Path);

	AllTexture.insert(std::make_pair(UpperName, LoadTexture));
}