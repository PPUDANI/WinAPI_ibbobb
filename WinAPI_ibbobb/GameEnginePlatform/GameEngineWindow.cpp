#include "GameEngineWindow.h"
#include <GameEngineBase/GameEngineDebug.h>

HINSTANCE GameEngineWindow::Instance = nullptr;

GameEngineWindow::GameEngineWindow()
{

}

GameEngineWindow::~GameEngineWindow()
{

}

void GameEngineWindow::Open()
{
    MyRegisterClass();
    InitInstance();
}

void GameEngineWindow::MyRegisterClass()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // LRESULT(CALLBACK* WNDPROC)(HWND, unsigned int, unsigned int, unsigned int);
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = Instance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "szWindowClass";
    wcex.hIconSm = nullptr;

    //윈도우 형식을등록하는 함수
    // 메뉴를 사용하지 않아.
    RegisterClassExW(&wcex);
}

LRESULT GameEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
    case WM_PAINT: // 화면에 그린다.
    {
        PAINTSTRUCT ps;

        // 윈도우 화면에 무언가를 그리기 위한 권한.
        HDC hdc = BeginPaint(hWnd, &ps);

        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY: // GetMessage() 함수에서 0을 리턴하게 만든다.
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void GameEngineWindow::InitInstance()
{
    // 윈도우를 만드는 함수
    hWnd = CreateWindowA(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        MsgBoxAssert("윈도우 생성에 실패했습니다.");
        return;
    }

    // API 구조는 개발자가 따로 기능을 만드는것이 아닌 Window가 개발자가 원하는 핸들을 제공하여
    // 그 핸들을 용도에 맞게 사용는 것이다.

    // 윈도우 출력
    ShowWindow(hWnd, nCmdShow);
    // 윈도우 갱신
    UpdateWindow(hWnd);
}

