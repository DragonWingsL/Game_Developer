//++++++++++++++++++++++++++++++++++
// ���ù�����
//----------------------------------

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

#include <windows.h>

class Util
{
public:
	static void DrawLine(HDC, int[4]);							// ����һ��ֱ��
	static void CreateDoubleBuffer(HWND, HDC &, HBITMAP &);		// ����������
	static void CreateLogFont(HFONT &, int);					// �����߼�����
		
};

#endif // !__UTIL_H__
