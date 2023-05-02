#pragma once

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

	bool WindowLoopOff()
	{
		IsWindowUpdate = false;
	}
protected:

private:

	static HINSTANCE Instance;
	std::string Title = "";
	HWND hWnd = nullptr;
	HDC Hdc = nullptr;
	static bool IsWindowUpdate;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void InitInstance();
	void MyRegisterClass();
};