#pragma once
#include <string>

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

	void TextureLoat(const std::string _Path)
	{

	}

	void TextureLoat(const std::string _Name, const std::string& _Path)
	{

	}

	GameEngineTexture* FindTexture(const std::string& _Image);
	bool IsLoadTexture(const std::string& _Image);
protected:

private:
	static ResourceManager Inst;

	ResourceManager();
	~ResourceManager();
};

