
#include "util.h"

void Util::DrawLine(HDC hdc, int points[4])
{
	/* *
	 * int[4] 表示两个点的 (x, y) 
	 * 第一个点为 (points[0], points[1])
	 * 第二个点为 (points[2], points[3])
	 * */

	::MoveToEx(hdc, points[0], points[1], NULL);
	::LineTo(hdc, points[2], points[3]);
}

void Util::CreateDoubleBuffer(HWND hWnd, HDC &mdc, HBITMAP &bitmap)
{
	/* * 
	 * 创建双缓冲
	 * 也就是: 兼容DC和兼容位图
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
	 * 创建逻辑字体
	 * */
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfHeight = nFontHeight;
	hFont = CreateFontIndirect(&logfont);
}