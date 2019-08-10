
//++++++++++++++++++++++++++++++++++
// �궨��
//----------------------------------
#ifndef UNICODE
#define UNICODE											// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#define _UNICODE										// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#endif

//++++++++++++++++++++++++++++++++++
// ͷ�ļ�
//----------------------------------
#include <windows.h>									// Win32��������Ҫ��ͷ�ļ�
#include <tchar.h>										// �����ַ���ͷ�ļ�

#include "GameType.h"
#include "util.h"
#include "Game.h"

//++++++++++++++++++++++++++++++++++
// ȫ�ֱ���
//----------------------------------
TCHAR g_lpszClassName[] = _T("Snake");					// �����������
TCHAR g_lpszWindowName[] = _T("Snake");					// ���ڵ�����,(Ҳ���Ǵ��ڵı���)
HDC g_mdc;												// ����DC
HBITMAP g_bitmap;										// ����λͼ
Game *g_game;											// ��Ϸ��

//++++++++++++++++++++++++++++++++++
// ��������
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// ������Ϣ�������
VOID SnakeGame_Init(HWND);								// ��ʼ����Ϸ
VOID SnakeGame_Render(HWND);							// ��Ⱦ��Ϸ
VOID SnakeGame_Update(DWORD dwDeltaTime);				// ������Ϸ
VOID SnakeGame_KeyDown(WPARAM);							// ��Ϸ���̰�������

//++++++++++++++++++++++++++++++++++
// ��Ϸ������
//----------------------------------
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT nCmdShow)
{
	/* 1.���һ�������� */
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

	/* 2.ע�ᴰ���� */
	if (RegisterClassEx(&wcex) == ((ATOM)0))
	{
		::MessageBox(NULL, _T("ע�ᴰ����ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.��������, ��������ʾ */
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
		::MessageBox(NULL, _T("��������ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.������ʾ���� */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* ��ʼ����Ϸ */
	SnakeGame_Init(hWnd);

	/* 5.��Ϣѭ�� */
	MSG msg = { 0 };
	DWORD dwLastTick = ::GetTickCount(), dwCurrentTick;
	while (msg.message != WM_QUIT)
	{
		// ��ǰʱ���
		dwCurrentTick = ::GetTickCount();
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// ��Ϸ����
		SnakeGame_Update(dwCurrentTick - dwLastTick);
		// ��Ϸ��Ⱦ
		SnakeGame_Render(hWnd);
		dwLastTick = dwCurrentTick;
		// Sleep ��Ϣ10���룬������ѭ�����Ĺ����CPU
		Sleep(10);
	}

	return msg.wParam;
}

//++++++++++++++++++++++++++++++++++
// ������Ϣ�������
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��Ϸ�жԴ��ڵ���Ϣ���д���
	if (g_game != NULL)
		g_game->WndProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_PAINT:
		// ��Ϸ��Ⱦ
		SnakeGame_Render(hWnd);
		break;
	case WM_KEYDOWN:
		// ��Ϸ��������
		SnakeGame_KeyDown(wParam);
		break;
	case WM_DESTROY:
		// �������٣��ͷ���Դ
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

	/* ����˫���� */
	Util::CreateDoubleBuffer(hWnd, g_mdc, g_bitmap);
	::SelectObject(g_mdc, g_bitmap);
}

VOID SnakeGame_Render(HWND hWnd)
{
	// ��Ϸ��Ⱦ
	if (g_mdc == NULL || g_game == NULL)
		return;

	HDC hdc = ::GetDC(hWnd);
	RECT rect;
	::GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ��Ϸ��Ⱦ
	g_game->Render(g_mdc);

	// ������DC���Ƶ��豸DC�� 
	::BitBlt(hdc, 0, 0, width, height, g_mdc, 0, 0, SRCCOPY);
	::ReleaseDC(hWnd, hdc);
}

VOID SnakeGame_Update(DWORD dwDeltaTime)
{
	/* ��Ϸ���� */
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

	/* ��Ϸ�������� */
	g_game->OnKeyDown(key);
}

