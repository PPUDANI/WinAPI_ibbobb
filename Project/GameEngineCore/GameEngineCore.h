#pragma once

#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineObject.h"

#include <windows.h>
#include <string>
#include <map>

class CoreProcess : public GameEngineObject
{

};
class GameEngineLevel;
class GameEngineCore
{
public:

	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	template<typename CoreProcessType>
	static void EngineStart(const std::string& _Title, HINSTANCE _Inst)
	{
		EngineStart(_Title, _Inst, new CoreProcessType());
	}

	template<typename LevelType>
	static void CreateLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		// 같은 이름의 Level이 있는지 검사
		if (AllLevel.end() != AllLevel.find(Upper))
		{
			MsgBoxAssert(Upper + "의 이름을 가진 GameEngineLevel은 이미 존재합니다.");
			return;
		}

		// 같은 이름의 레벨이 없다면 레벨 생성
		GameEngineLevel* NewLevel = new LevelType();

		LevelInit(NewLevel);
		AllLevel.insert(std::make_pair(Upper, NewLevel));
	}

	static void ChangeLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		std::map<std::string, GameEngineLevel*>::iterator Finder = AllLevel.find(Upper);

		if (AllLevel.end() == Finder)
		{
			MsgBoxAssert(Upper + "의 이름을 가진 GameEngineLevel은 존재하지 않습니다.");
			return;
		}

		NextLevel = Finder->second;
	}

protected:

private:
	static CoreProcess* Process;
	static std::string WindowTitle;
	static GameEngineLevel* CurLevel;
	static GameEngineLevel* NextLevel;

	static std::map<std::string, GameEngineLevel*> AllLevel;

	static void LevelInit(GameEngineLevel* _Level);
	static void CoreStart(HINSTANCE _Inst);
	static void CoreUpdate();
	static void CoreEnd();
	static void EngineStart(const std::string& _Title, HINSTANCE _Inst, CoreProcess* _Ptr);

	GameEngineCore();
	~GameEngineCore();
};

