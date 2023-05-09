#pragma once
#include <GameEngineBase/GameEngineMath.h>

#include <Windows.h>
#include <string>
class GameEngineWindowTexture
{
public:
	GameEngineWindowTexture();
	~GameEngineWindowTexture();

	GameEngineWindowTexture(const GameEngineWindowTexture& _Other) = delete;
	GameEngineWindowTexture(GameEngineWindowTexture&& _Other) noexcept = delete;
	GameEngineWindowTexture& operator=(const GameEngineWindowTexture& _Other) = delete;
	GameEngineWindowTexture& operator=(GameEngineWindowTexture&& _Other) noexcept = delete;

	void ResLoad(const std::string& _Path);
	void ResCreate(const float4& _Scale);

	HDC GetImageDC()
	{
		return ImageDC;
	}

	float4 GetScale();

	void BitCopy(GameEngineWindowTexture* _CopyTexture, const float4& Pos);
	void BitCopy(GameEngineWindowTexture* _CopyTexture, const float4& Pos, const float4& Scale);

protected:

private:
	HBITMAP BitMap;
	HBITMAP OldBitMap;
	HDC ImageDC;

	BITMAP Info;
	
	void ScaleCheck();
};

