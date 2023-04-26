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

    //������ ����������ϴ� �Լ�
    // �޴��� ������� �ʾ�.
    RegisterClassExW(&wcex);
}

LRESULT GameEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
    case WM_PAINT: // ȭ�鿡 �׸���.
    {
        PAINTSTRUCT ps;

        // ������ ȭ�鿡 ���𰡸� �׸��� ���� ����.
        HDC hdc = BeginPaint(hWnd, &ps);

        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY: // GetMessage() �Լ����� 0�� �����ϰ� �����.
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void GameEngineWindow::InitInstance()
{
    // �����츦 ����� �Լ�
    hWnd = CreateWindowA(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        MsgBoxAssert("������ ������ �����߽��ϴ�.");
        return;
    }

    // API ������ �����ڰ� ���� ����� ����°��� �ƴ� Window�� �����ڰ� ���ϴ� �ڵ��� �����Ͽ�
    // �� �ڵ��� �뵵�� �°� ���� ���̴�.

    // ������ ���
    ShowWindow(hWnd, nCmdShow);
    // ������ ����
    UpdateWindow(hWnd);
}

