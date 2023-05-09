#pragma once
#include "GameEngineWindowTexture.h"

#include <windows.h>
#include <string>

class GameEngineWindow
{
public:
	
	static GameEngineWindow MainWindow;

	GameEngineWindow();
	~GameEngineWindow();

	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;

	static void MessageLoop(HINSTANCE _Inst, void(*_Start)(HINSTANCE), void(*_Update)(), void(*_End)());
	void Open(const std::string& _Title, HINSTANCE hInstance);

	HDC GetHDC()
	{
		return Hdc;
	}

	float4 GetScale()
	{
		return Scale;
	}

	GameEngineWindowTexture* GetBackBuffer()
	{
		return  BackBuffer;
	}

	void SetPosAndScale(const float4& _Pos, const float4& _Scale);
	void DoubleBuffering();

	static void WindowLoopOff()
	{
		IsWindowUpdate = false;
	}

protected:

private:
	static bool IsWindowUpdate;
	static HINSTANCE Instance;

	HWND hWnd = nullptr;
	HDC Hdc = nullptr;

	std::string Title = "";
	float4 Scale;

	GameEngineWindowTexture* WindowBuffer = nullptr;
	GameEngineWindowTexture* BackBuffer = nullptr;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void InitInstance();
	void MyRegisterClass();
};