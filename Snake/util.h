//++++++++++++++++++++++++++++++++++
// 常用工具类
//----------------------------------

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

// 链接 msimg32.lib库, 该库包含了图像处理的函数接口,比如我们当前用到的AlphaBlend
#pragma comment (lib, "msimg32.lib")

// 安全释放对象
#define SafeDeleteObject(object) if (object != NULL) { delete object; object = NULL; }
// 安全释放数组对象
#define SafeDeleteArrayObject(object) if (object != NULL) { delete[] object; object = NULL; }
// 安全释放GDI对象
#define SafeDeleteGDIObject(object) if (object != NULL) { ::DeleteObject(object); object = NULL; }

class Util
{
public:
#pragma region TicTacToe
	// TicTacToe
	// 绘制一条直线
	static void DrawLine(HDC hdc, int points[4]);
	// 创建双缓冲
	static void CreateDoubleBuffer(HWND hWnd, HDC &hdc, HBITMAP &hBitmap);
	// 创建逻辑字体
	static void CreateLogFont(HFONT &hFont, int nFontHeight);
#pragma endregion

#pragma region Snake 新增
	// Snake 新增
	// 多边形填充
	static void PolyFill(HDC hdc, COLORREF color, LPPOINT points, const int c);
	// 计算两点间距离
	static double GetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2);
	// 获取字符串的宽度
	static BOOL GetStringWidth(HDC hdc, HFONT hFont, LPCTSTR lpString, int *pWidth);
	// 透明混合
	static BOOL AlphaBlend(HDC hdc, int x, int y, int w, int h, COLORREF blendColor, int nBlendAlpha);
	// 判断点是否在矩形内
	static bool CheckRectangleContainsPoint(int left, int top, int right, int bottom, int px, int py);
#pragma endregion
	
public:
	static const double PI;		// 圆周率
	static const double RAD;	// 弧度
};

#endif // !__UTIL_H__
