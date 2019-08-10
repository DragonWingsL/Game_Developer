
//++++++++++++++++++++++++++++++++++
// 宏定义
//----------------------------------
#ifndef UNICODE
#define UNICODE											// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#define _UNICODE										// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#endif

//++++++++++++++++++++++++++++++++++
// 头文件
//----------------------------------
#include <windows.h>									// Win32程序最重要的头文件
#include <tchar.h>										// 兼容字符集头文件

#include "GameType.h"
#include "util.h"
#include "Game.h"

//++++++++++++++++++++++++++++++++++
// 全局变量
//----------------------------------
TCHAR g_lpszClassName[] = _T("Snake");					// 窗口类的名称
TCHAR g_lpszWindowName[] = _T("Snake");					// 窗口的名称,(也就是窗口的标题)
HDC g_mdc;												// 兼容DC
HBITMAP g_bitmap;										// 兼容位图
Game *g_game;											// 游戏类

//++++++++++++++++++++++++++++++++++
// 函数声明
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 窗口消息处理过程
VOID SnakeGame_Init(HWND);								// 初始化游戏
VOID SnakeGame_Render(HWND);							// 渲染游戏
VOID SnakeGame_Update(DWORD dwDeltaTime);				// 更新游戏
VOID SnakeGame_KeyDown(WPARAM);							// 游戏键盘按键处理

//++++++++++++++++++++++++++++++++++
// 游戏主函数
//----------------------------------
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT nCmdShow)
{
	/* 1.设计一个窗口类 */
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_lpszClassName;

	/* 2.注册窗口类 */
	if (RegisterClassEx(&wcex) == ((ATOM)0))
	{
		::MessageBox(NULL, _T("注册窗口类失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.创建窗口, 并居中显示 */
	int cx = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	HWND hWnd = CreateWindowEx(
		NULL,
		g_lpszClassName,
		g_lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		(cx - 650) / 2,
		(cy - 650) / 2,
		650, 650,
		NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		::MessageBox(NULL, _T("创建窗口失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.更新显示窗口 */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* 初始化游戏 */
	SnakeGame_Init(hWnd);

	/* 5.消息循环 */
	MSG msg = { 0 };
	DWORD dwLastTick = ::GetTickCount(), dwCurrentTick;
	while (msg.message != WM_QUIT)
	{
		// 当前时间戳
		dwCurrentTick = ::GetTickCount();
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// 游戏更新
		SnakeGame_Update(dwCurrentTick - dwLastTick);
		// 游戏渲染
		SnakeGame_Render(hWnd);
		dwLastTick = dwCurrentTick;
		// Sleep 休息10毫秒，避免主循环消耗过多的CPU
		Sleep(10);
	}

	return msg.wParam;
}

//++++++++++++++++++++++++++++++++++
// 窗口消息处理过程
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 游戏中对窗口的消息进行处理
	if (g_game != NULL)
		g_game->WndProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_PAINT:
		// 游戏渲染
		SnakeGame_Render(hWnd);
		break;
	case WM_KEYDOWN:
		// 游戏按键处理
		SnakeGame_KeyDown(wParam);
		break;
	case WM_DESTROY:
		// 窗口销毁，释放资源
		::DeleteDC(g_mdc); g_mdc = NULL;
		SafeDeleteGDIObject(g_bitmap);
		SafeDeleteObject(g_game);
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	return ((LRESULT)0);
}

VOID SnakeGame_Init(HWND hWnd)
{
	g_game = new Game();
	g_game->Init(hWnd);

	/* 创建双缓冲 */
	Util::CreateDoubleBuffer(hWnd, g_mdc, g_bitmap);
	::SelectObject(g_mdc, g_bitmap);
}

VOID SnakeGame_Render(HWND hWnd)
{
	// 游戏渲染
	if (g_mdc == NULL || g_game == NULL)
		return;

	HDC hdc = ::GetDC(hWnd);
	RECT rect;
	::GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 游戏渲染
	g_game->Render(g_mdc);

	// 将兼容DC绘制到设备DC上 
	::BitBlt(hdc, 0, 0, width, height, g_mdc, 0, 0, SRCCOPY);
	::ReleaseDC(hWnd, hdc);
}

VOID SnakeGame_Update(DWORD dwDeltaTime)
{
	/* 游戏更新 */
	if (g_game == NULL)
		return;

	g_game->Update(dwDeltaTime);
}

VOID SnakeGame_KeyDown(WPARAM wParam)
{
	if (g_game == NULL)
		return;

	Keys key = Keys::KEY_W;
	if (wParam == 'W')
		key = Keys::KEY_W;
	else if (wParam == 'S')
		key = Keys::KEY_S;
	else if (wParam == 'A')
		key = Keys::KEY_A;
	else if (wParam == 'D')
		key = Keys::KEY_D;

	else if (wParam == VK_UP)
		key = Keys::KEY_UP;
	else if (wParam == VK_DOWN)
		key = Keys::KEY_DOWN;
	else if (wParam == VK_LEFT)
		key = Keys::KEY_LEFT;
	else if (wParam == VK_RIGHT)
		key = Keys::KEY_RIGHT;

	/* 游戏按键处理 */
	g_game->OnKeyDown(key);
}

