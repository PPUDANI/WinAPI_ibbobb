#pragma once
#include <string>
#include <map>
#include <GameEngineBase/GameEnginePath.h>
class GameEngineTexture;
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

	void TextureLoad(const std::string& _Path)
	{
		GameEnginePath LoadPath = _Path;
		TextureLoad(LoadPath.GetFileName(), _Path);
	}

	void TextureLoad(const std::string _Name, const std::string& _Path);

	GameEngineTexture* FindTexture(const std::string& _Name);
	bool IsLoadTexture(const std::string& _Name);

protected:

private:
	static ResourceManager Inst;

	ResourceManager();
	~ResourceManager();

	std::map<std::string, GameEngineTexture*> AllTexture;
};

