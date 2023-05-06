#pragma once
#include <Windows.h>
#include <string>
class GameEngineTexture
{
public:
	GameEngineTexture();
	~GameEngineTexture();

	GameEngineTexture(const GameEngineTexture& _Other) = delete;
	GameEngineTexture(GameEngineTexture&& _Other) noexcept = delete;
	GameEngineTexture& operator=(const GameEngineTexture& _Other) = delete;
	GameEngineTexture& operator=(GameEngineTexture&& _Other) noexcept = delete;

	void ResLoad(const std::string& _Path);

	HDC GetImageDC()
	{
		return ImageDC;
	}

protected:

private:
	HBITMAP BitMap;
	HBITMAP OldBitMap;
	HDC ImageDC;

	BITMAP info;
	
	void ScaleCheck();
};

