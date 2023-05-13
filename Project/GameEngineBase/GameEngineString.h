#pragma once
#include <string>

// 게임 파일 및 레벨의 이름을 관리하기 위한 클래스
class GameEngineString
{
public:
	// constrcuter destructer
	GameEngineString();
	~GameEngineString();

	// delete Function
	GameEngineString(const GameEngineString& _Other) = delete;
	GameEngineString(GameEngineString&& _Other) noexcept = delete;
	GameEngineString& operator=(const GameEngineString& _Other) = delete;
	GameEngineString& operator=(GameEngineString&& _Other) noexcept = delete;

	// 모든 문자를 대문자로 반환하는 함수
	static std::string ToUpperReturn(const std::string& _Value) 
	{
		std::string UpperString = _Value;

		size_t StrSize = UpperString.size();
		for (size_t i = 0; i < StrSize; i++)
		{
			UpperString[i] = toupper(UpperString[i]);
		}

		return UpperString;
	}

protected:

private:

};

