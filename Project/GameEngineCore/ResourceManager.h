#pragma once
#include <GameEngineBase/GameEnginePath.h>

#include <string>
#include <map>

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
	bool IsLoadTexture(const std::string& _Name);

protected:

private:
	static ResourceManager Inst;

	ResourceManager();
	~ResourceManager();

	std::map<std::string, GameEngineWindowTexture*> AllTexture;
};

