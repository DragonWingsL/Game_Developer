
#include <windows.h>		// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)

#include "util.h"			// ���ù�����
#include "Fruit.h"			// ʳ����

Fruit::Fruit(int score, int x, int y, int width, int height) 
	: m_score(score),	// ��ʼ��ʳ��ķ���
	m_x(x),				// ��ʼ��ʳ���x����λ��
	m_y(y),				// ��ʼ��ʳ���y����λ��
	m_w(width),			// ��ʼ��ʳ��Ŀ��
	m_h(height)			// ��ʼ��ʳ��ĸ߶�
{
}

Fruit::~Fruit()
{
}

void Fruit::Render(HDC hdc)
{
	// ����ʳ��

	// �����ջ���
	HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(NULL_PEN));
	// ������ˢ,���ݷ���������ͬ����ɫ
	HBRUSH hBrush = ::CreateSolidBrush(GetScore() > 1 ? RGB(255, 0, 0) : RGB(0, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// ����ʳ�� "��"
	::Ellipse(hdc, (int)(m_x - m_w * .5f), (int)(m_y - m_h * .5f), (int)(m_x + m_w * .5f), (int)(m_y + m_h * .5f));

	::SelectObject(hdc, hOldPen);
	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hBrush);
}

int Fruit::GetX()
{
	// ��ȡʳ���x����λ��
	return m_x;
}

int Fruit::GetY()
{
	// ��ȡʳ���y����λ��
	return m_y;
}

void Fruit::Reset(int score, int x, int y, int w, int h)
{
	// ����ʳ���λ�ÿ���Լ�����
	m_score = score;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

int Fruit::GetScore()
{
	// ��ȡʳ��ķ���
	return m_score;
}
