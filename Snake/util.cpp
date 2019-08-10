
#include <windows.h>
#include <cmath>

#include "util.h"

const double Util::PI = 3.1415926;
const double Util::RAD = 3.1415926 / 180.0;

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

void Util::PolyFill(HDC hdc, COLORREF color, LPPOINT points, const int c)
{
	/* *
	 * 通过一组点进行相连,进行填充
	 * 每两个元素为一个坐标(x, y)
	 * */
	
	/* 设置接收定点类型 */
	BYTE *type_arr = new BYTE[c];
	::memset((void*)type_arr, 0, sizeof(BYTE) * c);

	int type_index = 1;
	type_arr[0] = PT_MOVETO;
	while (true)
	{
		if (type_index >= c)
			break;
		type_arr[type_index] = PT_LINETO;
		++type_index;
	}
	type_arr[c-1] |= PT_CLOSEFIGURE;
	
	/* 进行绘制路径 */
	::BeginPath(hdc);
	::PolyDraw(hdc, points, type_arr, c);
	::EndPath(hdc);

	/* 填充路径 */
	HBRUSH hBrush = ::CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);
	::FillPath(hdc);
	::SelectObject(hdc, hOldBrush);
	::DeleteObject(hBrush); hBrush = NULL;
	SafeDeleteArrayObject(type_arr);
}

double Util::GetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2)
{
	// 根据勾股定理计算两点距离
	double a = x1 - x2;
	double b = y1 - y2;
	return sqrt(a * a + b * b);
}

bool Util::CheckRectangleContainsPoint(int left, int top, int right, int bottom, int px, int py)
{
	// 通过计算点与矩形左上角和右下角比较是否在矩形中
	return (px >= left && px <= right && py >= top && py <= bottom);
}

BOOL Util::GetStringWidth(HDC hdc, HFONT hFont, LPCTSTR lpString, int *pWidth)
{
	BOOL bRet = TRUE;

	*pWidth = 0;
	int nWidth = 0;

	// 选择字体
	HFONT hOldFont;
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	// 遍历字符串
	for (int i = 0, count = wcslen(lpString); i < count; ++i)
	{
		// 获取单个字符的宽度
		bRet = GetCharWidth(hdc, lpString[i], lpString[i], &nWidth);
		if (!bRet)
			return bRet;
		// 累加宽度
		*pWidth += nWidth;
	}

	::SelectObject(hdc, hOldFont);
	return bRet;
}

BOOL Util::AlphaBlend(HDC hdc, int x, int y, int w, int h, COLORREF blendColor, int nBlendAlpha)
{
	// 透明混合

	BOOL bRet = TRUE;
	// 创建兼容DC
	HDC mdc = ::CreateCompatibleDC(hdc);
	// 创建兼容位图
	HBITMAP hBitmap = ::CreateCompatibleBitmap(mdc, w, h);
	// 选择兼容位置
	::SelectObject(mdc, hBitmap);

	// 混合区域
	RECT rcBlend;
	rcBlend.left = x;
	rcBlend.top = y;
	rcBlend.right = x + w;
	rcBlend.bottom = y + h;

	// 填充混合颜色
	::FillRect(mdc, &rcBlend, ::CreateSolidBrush(blendColor));

	// 设置混合操作的结构体
	BLENDFUNCTION bf;
	// 设置混合操作
	bf.BlendOp = AC_SRC_OVER;
	// 设置混合标志(必须为0)
	bf.BlendFlags = 0;
	// 设置透明度(0-255)
	bf.SourceConstantAlpha = nBlendAlpha;
	// 设置混合方式
	bf.AlphaFormat = 0;

	// 进行混合
	bRet = ::AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, bf);
	
	// 释放DC
	::DeleteDC(mdc); mdc = NULL;
	// 释放位图
	SafeDeleteGDIObject(hBitmap);
	return bRet;
}

