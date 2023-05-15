#pragma once
#include "GameEnginePath.h"

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

	// std::vector<GameEngineFile> GetAllFile();
protected:

private:

};

