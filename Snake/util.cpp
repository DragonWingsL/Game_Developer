
#include <windows.h>
#include <cmath>

#include "util.h"

const double Util::PI = 3.1415926;
const double Util::RAD = 3.1415926 / 180.0;

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

void Util::PolyFill(HDC hdc, COLORREF color, LPPOINT points, const int c)
{
	/* *
	 * ͨ��һ����������,�������
	 * ÿ����Ԫ��Ϊһ������(x, y)
	 * */
	
	/* ���ý��ն������� */
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
	
	/* ���л���·�� */
	::BeginPath(hdc);
	::PolyDraw(hdc, points, type_arr, c);
	::EndPath(hdc);

	/* ���·�� */
	HBRUSH hBrush = ::CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);
	::FillPath(hdc);
	::SelectObject(hdc, hOldBrush);
	::DeleteObject(hBrush); hBrush = NULL;
	SafeDeleteArrayObject(type_arr);
}

double Util::GetDistanceBetweenTwoPoint(double x1, double y1, double x2, double y2)
{
	// ���ݹ��ɶ�������������
	double a = x1 - x2;
	double b = y1 - y2;
	return sqrt(a * a + b * b);
}

bool Util::CheckRectangleContainsPoint(int left, int top, int right, int bottom, int px, int py)
{
	// ͨ���������������ϽǺ����½ǱȽ��Ƿ��ھ�����
	return (px >= left && px <= right && py >= top && py <= bottom);
}

BOOL Util::GetStringWidth(HDC hdc, HFONT hFont, LPCTSTR lpString, int *pWidth)
{
	BOOL bRet = TRUE;

	*pWidth = 0;
	int nWidth = 0;

	// ѡ������
	HFONT hOldFont;
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	// �����ַ���
	for (int i = 0, count = wcslen(lpString); i < count; ++i)
	{
		// ��ȡ�����ַ��Ŀ��
		bRet = GetCharWidth(hdc, lpString[i], lpString[i], &nWidth);
		if (!bRet)
			return bRet;
		// �ۼӿ��
		*pWidth += nWidth;
	}

	::SelectObject(hdc, hOldFont);
	return bRet;
}

BOOL Util::AlphaBlend(HDC hdc, int x, int y, int w, int h, COLORREF blendColor, int nBlendAlpha)
{
	// ͸�����

	BOOL bRet = TRUE;
	// ��������DC
	HDC mdc = ::CreateCompatibleDC(hdc);
	// ��������λͼ
	HBITMAP hBitmap = ::CreateCompatibleBitmap(mdc, w, h);
	// ѡ�����λ��
	::SelectObject(mdc, hBitmap);

	// �������
	RECT rcBlend;
	rcBlend.left = x;
	rcBlend.top = y;
	rcBlend.right = x + w;
	rcBlend.bottom = y + h;

	// �������ɫ
	::FillRect(mdc, &rcBlend, ::CreateSolidBrush(blendColor));

	// ���û�ϲ����Ľṹ��
	BLENDFUNCTION bf;
	// ���û�ϲ���
	bf.BlendOp = AC_SRC_OVER;
	// ���û�ϱ�־(����Ϊ0)
	bf.BlendFlags = 0;
	// ����͸����(0-255)
	bf.SourceConstantAlpha = nBlendAlpha;
	// ���û�Ϸ�ʽ
	bf.AlphaFormat = 0;

	// ���л��
	bRet = ::AlphaBlend(hdc, 0, 0, w, h, mdc, 0, 0, w, h, bf);
	
	// �ͷ�DC
	::DeleteDC(mdc); mdc = NULL;
	// �ͷ�λͼ
	SafeDeleteGDIObject(hBitmap);
	return bRet;
}

