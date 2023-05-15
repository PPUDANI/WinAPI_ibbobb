#include "ResourceManager.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include "GameEngineSprite.h"

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

	for (const std::pair<std::string, GameEngineSprite*>& Pair : AllSprite)
	{
		GameEngineSprite* Sprite = Pair.second;

		if (nullptr != Sprite)
		{
			delete Sprite;
			Sprite = nullptr;
		}
	}
}

bool ResourceManager::IsLoadTexture(const std::string& _Name)
{
	return FindTexture(_Name) != nullptr;
}

GameEngineWindowTexture* ResourceManager::TextureLoad(const std::string _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);

	GameEngineWindowTexture* LoadTexture = new GameEngineWindowTexture();

	LoadTexture->ResLoad(_Path);

	AllTexture.insert(std::make_pair(UpperName, LoadTexture));

	return LoadTexture;
}

GameEngineSprite* ResourceManager::FindSprite(const std::string& _Name)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_Name);


	std::map<std::string, GameEngineSprite*>::iterator FindIter = AllSprite.find(UpperName);

	if (FindIter == AllSprite.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

GameEngineSprite* ResourceManager::CreateSpriteFolder(const std::string& _SpriteName, const std::string& _Path)
{
	GameEngineDirectory Directory = _Path;

	//for (size_t i = 0; i < length; i++)
	//{
		// LoadTexture();
	//}

	return nullptr;
}

GameEngineSprite* ResourceManager::CreateSpriteSheet(
	const std::string& _SpriteName,
	const std::string& _TexturePath, 
	int _XCount, int _YCount)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_SpriteName);

	if (nullptr != FindSprite(UpperName))
	{
		MsgBoxAssert("이미 로드한 스프라이트를 또 로드하려고 했습니다.");
	}

	GameEnginePath Path = _TexturePath;

	GameEngineWindowTexture* Texture = ResourceManager::FindTexture(Path.GetFileName());

	if (nullptr == Texture)
	{
		Texture = ResourceManager::TextureLoad(_TexturePath);
	}

	float4 Scale = Texture->GetScale();

	GameEngineSprite* NewSprite = new GameEngineSprite();

	NewSprite->CreateSpriteSheet(Texture, _XCount, _YCount);

	AllSprite.insert(std::make_pair(UpperName, NewSprite));

	return NewSprite;
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

