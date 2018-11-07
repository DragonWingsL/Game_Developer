
//++++++++++++++++++++++++++++++++++
// �궨��
//----------------------------------
#define UNICODE											// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#define _UNICODE										// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����

//++++++++++++++++++++++++++++++++++
// ͷ�ļ�
//----------------------------------
#include <windows.h>									// Win32��������Ҫ��ͷ�ļ�
#include <tchar.h>										// �����ַ���ͷ�ļ�

#include "game.h"										// ��Ϸͷ�ļ�
#include "util.h"

//++++++++++++++++++++++++++++++++++
// ȫ�ֱ���
//----------------------------------
WCHAR g_lpszClassName[] = _T("TicTacToe");				// �����������
WCHAR g_lpszWindowName[] = _T("TicTacToe");				// ���ڵ�����,(Ҳ���Ǵ��ڵı���)
Game g_game;											// ��Ϸ����
HDC g_mdc;												// ����DC
HBITMAP g_bitmap;										// ����λͼ

//++++++++++++++++++++++++++++++++++
// ��������
//----------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// ������Ϣ�������
VOID TicTacToe_Init(HWND);								// ������Ϸ��ʼ��
VOID TicTacToe_Render(HWND);							// ������Ϸ����
VOID TicTacToe_MouseDown(int, int);						// ������Ϸ���������

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
		MessageBox(NULL, _T("ע�ᴰ����ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 3.�������� */
	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName, g_lpszWindowName, WS_OVERLAPPEDWINDOW, 10, 10, 500, 500, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, _T("��������ʧ��!"), _T("����"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	/* 4.������ʾ���� */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* ������Ϸ�ĳ�ʼ�� */
	TicTacToe_Init(hWnd);

	/* 5.��Ϣѭ�� */
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		/* ������Ϸ��Ⱦ */
		TicTacToe_Render(hWnd);
	}

	return msg.wParam;
}


//++++++++++++++++++++++++++++++++++
// ������Ϣ�������
//----------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		/* ������Ϸ��Ⱦ */
		TicTacToe_Render(hWnd);
		break;
	case WM_LBUTTONDOWN:
		/* ������Ϸ�������Ϣ���� */
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
// ������Ϸ��ʼ��
//----------------------------------
VOID TicTacToe_Init(HWND hWnd)
{
	/* ��Ϸ��ʼ�� */
	g_game.Init(hWnd);
	/* ��������DC�ͼ���λͼ */
	Util::CreateDoubleBuffer(hWnd, g_mdc, g_bitmap);
	::SelectObject(g_mdc, g_bitmap);
}

//++++++++++++++++++++++++++++++++++
// ������Ϸ����
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

	/* ��Ϸ���� */
	g_game.Render(g_mdc);

	/* ������DC���Ƶ��豸DC�� */
	::BitBlt(hdc, 0, 0, width, height, g_mdc, 0, 0, SRCCOPY);
	::ReleaseDC(hWnd, hdc);
}

//++++++++++++++++++++++++++++++++++
// ������Ϸ�����
//----------------------------------
VOID TicTacToe_MouseDown(int x, int y)
{
	/* ��Ϸ��괦�� */
	g_game.MouseDown(x, y);
}
