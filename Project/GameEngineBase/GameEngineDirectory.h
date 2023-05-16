#pragma once
#include "GameEngineFile.h"

class GameEngineDirectory : public GameEnginePath
{
public:
	GameEngineDirectory();
	GameEngineDirectory(const std::string& _path);
	~GameEngineDirectory();

	GameEngineDirectory(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory(GameEngineDirectory&& _Other) noexcept = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory& operator=(GameEngineDirectory&& _Other) noexcept = delete;

	std::vector<class GameEngineFile> GetAllFile(std::vector<std::string> _Ext);

protected:

private:

};

