#pragma once
#include "GameEnginePath.h"
#include <string>
class GameEngineFile : public GameEnginePath
{
public:
	GameEngineFile();
	GameEngineFile(const std::string& _path);
	~GameEngineFile();

	//GameEngineFile(const GameEngineFile& _Other) = delete;
	//GameEngineFile(GameEngineFile&& _Other) noexcept = delete;
	//GameEngineFile& operator=(const GameEngineFile& _Other) = delete;
	//GameEngineFile& operator=(GameEngineFile&& _Other) noexcept = delete;
protected:

private:

};

