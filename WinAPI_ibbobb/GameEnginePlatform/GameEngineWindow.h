#pragma once

#include <windows.h>
#include <string>

class GameEngineWindow
{
public:
	GameEngineWindow();
	~GameEngineWindow();

	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;

	void Open();
protected:

private:
	static HINSTANCE Instance;
	std::string Title = "";
	HWND hWnd = nullptr;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void InitInstance();
	void MyRegisterClass();
	
};

