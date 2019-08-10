//++++++++++++++++++++++++++++++++++
// ���ù�����
//----------------------------------

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

// ���� msimg32.lib��, �ÿ������ͼ����ĺ����ӿ�,�������ǵ�ǰ�õ���AlphaBlend
#pragma comment (lib, "msimg32.lib")

// ��ȫ�ͷŶ���
#define SafeDeleteObject(object) if (object != NULL) { delete object; object = NULL; }
// ��ȫ�ͷ��������
#define SafeDeleteArrayObject(object) if (object != NULL) { delete[] object; object = NULL; }
// ��ȫ�ͷ�GDI����
#define SafeDeleteGDIObject(object) if (object != NULL) { ::DeleteObject(object); object = NULL; }

class Util
{
public:
#pragma region TicTacToe
	// TicTacToe
	// ����һ��ֱ��
	static void DrawLine(HDC hdc, int points[4]);
	// ����˫����
	static void CreateDoubleBuffer(HWND hWnd, HDC &hdc, HBITMAP &hBitmap);
	// �����߼�����
	static void CreateLogFont(HFONT &hFont, int nFontHeight);
#pragma endregion

#pragma region Snake ����
	// Snake ����
	// ��������
	static void PolyFill(HDC hdc, COLORREF color, LPPOINT points, const int c);
	// ������������
	static double GetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2);
	// ��ȡ�ַ����Ŀ��
	static BOOL GetStringWidth(HDC hdc, HFONT hFont, LPCTSTR lpString, int *pWidth);
	// ͸�����
	static BOOL AlphaBlend(HDC hdc, int x, int y, int w, int h, COLORREF blendColor, int nBlendAlpha);
	// �жϵ��Ƿ��ھ�����
	static bool CheckRectangleContainsPoint(int left, int top, int right, int bottom, int px, int py);
#pragma endregion
	
public:
	static const double PI;		// Բ����
	static const double RAD;	// ����
};

#endif // !__UTIL_H__
