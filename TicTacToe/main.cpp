
//++++++++++++++++++++++++++++++++++
// 宏定义
//----------------------------------
#define UNICODE											// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#define _UNICODE										// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免

//++++++++++++++++++++++++++++++++++
// 头文件
//----------------------------------
#include <windows.h>									// Win32程序最重要的头文件
#include <tchar.h>										// 兼容字符集头文件

#include "game.h"										// 游戏头文件
#include "util.h"

//++++++++++++++++++++++++++++++++++
// 全局变量
//----------------------------------
WCHAR g_lpszClassName[] = _T("TicTacToe");				// 窗口类的名称
WCHAR g_lpszWindowName[] = _T("TicTacToe");				// 窗口的名称,(也就是窗口的标题)
Game g_game;											// 游戏对象
HDC g_mdc;												// 兼容DC
HBITMAP g_bitmap;										// 兼容位图

//++++++++++++++++++++++++++++++++++
// 函数声明
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 窗口消息处理过程
VOID TicTacToe_Init(HWND);								// 井字游戏初始化
VOID TicTacToe_Render(HWND);							// 井字游戏绘制
VOID TicTacToe_MouseDown(int, int);						// 井字游戏鼠标点击处理

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
		MessageBox(NULL, _T("注册窗口类失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.创建窗口 */
	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName, g_lpszWindowName, WS_OVERLAPPEDWINDOW, 10, 10, 500, 500, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, _T("创建窗口失败!"), _T("错误"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.更新显示窗口 */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* 井字游戏的初始化 */
	TicTacToe_Init(hWnd);

	/* 5.消息循环 */
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		/* 井字游戏渲染 */
		TicTacToe_Render(hWnd);
	}

	return msg.wParam;
}


//++++++++++++++++++++++++++++++++++
// 窗口消息处理过程
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		/* 井字游戏渲染 */
		TicTacToe_Render(hWnd);
		break;
	case WM_LBUTTONDOWN:
		/* 井字游戏鼠标点击消息处理 */
		TicTacToe_MouseDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return ((LRESULT)0);
}

//++++++++++++++++++++++++++++++++++
// 井字游戏初始化
//----------------------------------
VOID TicTacToe_Init(HWND hWnd)
{
	/* 游戏初始化 */
	g_game.Init(hWnd);
	/* 创建兼容DC和兼容位图 */
	Util::CreateDoubleBuffer(hWnd, g_mdc, g_bitmap);
	::SelectObject(g_mdc, g_bitmap);
}

//++++++++++++++++++++++++++++++++++
// 井字游戏绘制
//----------------------------------
VOID TicTacToe_Render(HWND hWnd)
{
	if (g_mdc == NULL)
		return;

	HDC hdc = ::GetDC(hWnd);
	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	/* 游戏绘制 */
	g_game.Render(g_mdc);

	/* 将兼容DC绘制到设备DC中 */
	::BitBlt(hdc, 0, 0, width, height, g_mdc, 0, 0, SRCCOPY);
	::ReleaseDC(hWnd, hdc);
}

//++++++++++++++++++++++++++++++++++
// 井字游戏鼠标点击
//----------------------------------
VOID TicTacToe_MouseDown(int x, int y)
{
	/* 游戏鼠标处理 */
	g_game.MouseDown(x, y);
}
