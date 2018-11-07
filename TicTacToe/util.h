//++++++++++++++++++++++++++++++++++
// 常用工具类
//----------------------------------

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

#include <windows.h>

class Util
{
public:
	static void DrawLine(HDC, int[4]);							// 绘制一条直线
	static void CreateDoubleBuffer(HWND, HDC &, HBITMAP &);		// 创建创缓冲
	static void CreateLogFont(HFONT &, int);					// 创建逻辑字体
		
};

#endif // !__UTIL_H__
