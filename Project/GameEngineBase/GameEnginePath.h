#pragma once
#include <string>
#include <filesystem>

class GameEnginePath
{
public:
	GameEnginePath();
	GameEnginePath(const std::string& _Path);
	~GameEnginePath();

	GameEnginePath(const GameEnginePath& _Other) = delete;
	GameEnginePath(GameEnginePath&& _Other) noexcept = delete;
	GameEnginePath& operator=(const GameEnginePath& _Other) = delete;
	GameEnginePath& operator=(GameEnginePath&& _Other) noexcept = delete;

	std::string GetFileName();

	void GetCurrentPath();
	void MoveParent();
	void MoveParentToExistsChild(const std::string& _ChildPath);
	void MoveChild(const std::string& _ChildPAth);

	std::string PlusFilePath(const std::string _ChildPath);

	std::string GetStringPath()
	{
		return Path.string();
	}

protected:

private:
	std::filesystem::path Path;
};

