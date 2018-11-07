
#include "util.h"

void Util::DrawLine(HDC hdc, int points[4])
{
	/* *
	 * int[4] ��ʾ������� (x, y) 
	 * ��һ����Ϊ (points[0], points[1])
	 * �ڶ�����Ϊ (points[2], points[3])
	 * */

	::MoveToEx(hdc, points[0], points[1], NULL);
	::LineTo(hdc, points[2], points[3]);
}

void Util::CreateDoubleBuffer(HWND hWnd, HDC &mdc, HBITMAP &bitmap)
{
	/* * 
	 * ����˫����
	 * Ҳ����: ����DC�ͼ���λͼ
	 * */
	HDC hdc = ::GetDC(hWnd);
	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);
	mdc = ::CreateCompatibleDC(hdc);
	bitmap = ::CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	::ReleaseDC(hWnd, hdc);
}

void Util::CreateLogFont(HFONT &hFont, int nFontHeight)
{
	/* * 
	 * �����߼�����
	 * */
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfHeight = nFontHeight;
	hFont = CreateFontIndirect(&logfont);
}