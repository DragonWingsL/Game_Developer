
#include <windows.h>		// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <tchar.h>			// ͨ���ַ���ͷ�ļ�,�����ַ���֮��ļ��ݱ���ASCII�����Unicode����

#include "Game.h"			// ��Ϸ��
#include "util.h"			// ���ù�����
#include "GameLabel.h"		// ��Ϸ��ǩ��

GameLabel::GameLabel(int x, int y, LPCTSTR text) 
	: m_x(x),			// ��ʼ����ǩ��xλ������
	m_y(y),				// ��ʼ����ǩ��y����λ��
	m_visible(true),	// ��ʼ����ǩ�Ŀɼ���
	m_enable(true)		// ��ʼ����ǩ������
{
	// ����Ĭ�������С
	SetFontSize(100);
	// �����ı�
	SetText(text == NULL ? _T("GameLabel") : text);
	// ���ñ�����ǰ��ɫ,Ĭ��Ϊ͸������
	SetBackground(RGB(0, 0, 0));
	SetForeground(RGB(255, 255, 255));
	SetBackgroundTransparent(true);
}

GameLabel::~GameLabel()
{
}

void GameLabel::Render(HDC hdc)
{
	// �ж��Ƿ�ɼ�
	if (!m_visible)
		return;

	HFONT hFont, hOldFont;
	Util::CreateLogFont(hFont, m_h);
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	int nOldBkMode;
	COLORREF oldBkColor, oldTextColor;
	if (m_background_transparent)
	{
		// ���ñ���͸��
		nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
	}
	else 
	{
		// ���ñ�����͸��
		nOldBkMode = ::SetBkMode(hdc, OPAQUE);
		// ���ñ���ɫ
		oldBkColor = ::SetBkColor(hdc, m_background);
	}

	// ����ǰ��ɫ
	oldTextColor = ::SetTextColor(hdc, m_foreground);
	// ��Ⱦ�ı�
	::TextOut(hdc, m_x, m_y, m_text, wcslen(m_text));

	::SetTextColor(hdc, oldTextColor);

	if (!m_background_transparent)
	{
		::SetBkColor(hdc, oldBkColor);
	}
	::SetBkMode(hdc, nOldBkMode);
	::SelectObject(hdc, hOldFont);
	SafeDeleteGDIObject(hFont);
}

void GameLabel::SetText(LPCTSTR text)
{
	if (text == NULL)
	{
		text = _T("");
	}

	m_text = text;

	int nWidth = 0;
	HDC hdc = ::GetDC(Game::GetGameHwnd());
	HFONT hFont;
	Util::CreateLogFont(hFont, m_h);
	// �����ı����
	Util::GetStringWidth(hdc, hFont, m_text, &m_w);
	::ReleaseDC(Game::GetGameHwnd(), hdc);
	SafeDeleteGDIObject(hFont);
}

void GameLabel::SetFontSize(int nHeight)
{
	// ���������С
	m_h = nHeight;
	// ���¼����ı����
	SetText(GetText());
}

void GameLabel::SetX(int x)
{
	m_x = x;
}

void GameLabel::SetY(int y)
{
	m_y = y;
}

void GameLabel::SetBackground(COLORREF background)
{
	m_background = background;
}

void GameLabel::SetForeground(COLORREF foreground)
{
	m_foreground = foreground;
}

void GameLabel::SetBackgroundTransparent(bool bTransparent)
{
	m_background_transparent = bTransparent;
}

void GameLabel::SetEnable(bool bEnable)
{
	m_enable = bEnable;
}

void GameLabel::SetVisible(bool bVisible)
{
	m_visible = bVisible;
}

LPCTSTR GameLabel::GetText()
{
	return m_text;
}

int GameLabel::GetWidth()
{
	return m_w;
}

int GameLabel::GetHeight()
{
	return m_h;
}

bool GameLabel::IsVisible()
{
	return m_visible;
}

bool GameLabel::IsEnable()
{
	return m_enable;
}

int GameLabel::GetX()
{
	return m_x;
}

int GameLabel::GetY()
{
	return m_y;
}
