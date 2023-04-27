#pragma once

#include <windows.h>
#include <string>
#include <map>

#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

class GameEngineLevel;
class GameEngineCore
{
public:
	GameEngineCore();
	~GameEngineCore();

	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	static void EngineStart(const std::string& _Title, HINSTANCE _Inst);

	template<typename LevelType>
	static void CreateLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		if (AllLevel.end() != AllLevel.find(Upper))
		{
			MsgBoxAssert(Upper + "의 이름을 가진 GameEngineLevel은 이미 존재합니다.");
			return;
		}

		GameEngineLevel* NewLevel = new LevelType();
		AllLevel.insert(std::make_pair(Upper, NewLevel));
	}
protected:

private:

	static std::string WindowTitle;
	static std::map<std::string, GameEngineLevel*> AllLevel;

	static void CoreStart(HINSTANCE _Inst);
	static void CoreUpdate();
	static void CoreEnd();

};

