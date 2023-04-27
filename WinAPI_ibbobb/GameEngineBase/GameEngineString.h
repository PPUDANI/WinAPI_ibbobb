#pragma once
#include <string>

// Ό³Έν :
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

