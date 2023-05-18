#pragma once
#include <GameEngineBase/GameEnginePath.h>

#include <string>
#include <map>

class GameEngineSprite;
class GameEngineWindowTexture;
class ResourceManager
{
public:

	ResourceManager(const ResourceManager& _Other) = delete;
	ResourceManager(ResourceManager&& _Other) noexcept = delete;
	ResourceManager& operator=(const ResourceManager& _Other) = delete;
	ResourceManager& operator=(ResourceManager&& _Other) noexcept = delete;

	static ResourceManager& GetInst()
	{
		return Inst;
	}

	GameEngineWindowTexture* TextureLoad(const std::string& _Path)
	{
		GameEnginePath LoadPath = _Path;
		return TextureLoad(LoadPath.GetFileName(), _Path);
	}
	GameEngineWindowTexture* TextureLoad(const std::string _Name, const std::string& _Path);


	GameEngineWindowTexture* FindTexture(const std::string& _Name);
	GameEngineSprite* FindSprite(const std::string& _Name);


	GameEngineSprite* CreateSpriteFolder(const std::string& _Path)
	{
		GameEnginePath FolderPath = _Path;
		return CreateSpriteFolder(FolderPath.GetFileName(), _Path);
	}
	GameEngineSprite* CreateSpriteFolder(const std::string& _SpriteName, const std::string& _Path);


	GameEngineSprite* CreateSpriteSheet(const std::string& _TexturePath, int _XCount, int _YCount)
	{
		GameEnginePath Path = _TexturePath;
		return CreateSpriteSheet(Path.GetFileName(), _TexturePath, _XCount, _YCount);
	}
	GameEngineSprite* CreateSpriteSheet(const std::string& _SpriteName, const std::string& _TexturePath, int _XCount, int _YCount);

	bool IsLoadTexture(const std::string& _Name);
	void TextureFileLoad(const std::string& _FileName, const std::string& _Path);
	void SpriteFileLoad(const std::string& _FileName, const std::string& _Path, int _XCount, int _YCount);



protected:

private:
	static ResourceManager Inst;

	ResourceManager();
	~ResourceManager();

	std::map<std::string, GameEngineWindowTexture*> AllTexture;
	std::map<std::string, GameEngineSprite*> AllSprite;
};

