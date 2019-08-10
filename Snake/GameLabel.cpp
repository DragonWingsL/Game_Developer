
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)
#include <tchar.h>			// 通用字符集头文件,对于字符集之间的兼容比如ASCII编码和Unicode编码

#include "Game.h"			// 游戏类
#include "util.h"			// 常用工具类
#include "GameLabel.h"		// 游戏标签类

GameLabel::GameLabel(int x, int y, LPCTSTR text) 
	: m_x(x),			// 初始化标签的x位置坐标
	m_y(y),				// 初始化标签的y坐标位置
	m_visible(true),	// 初始化标签的可见性
	m_enable(true)		// 初始化标签的启用
{
	// 设置默认字体大小
	SetFontSize(100);
	// 设置文本
	SetText(text == NULL ? _T("GameLabel") : text);
	// 设置背景和前景色,默认为透明背景
	SetBackground(RGB(0, 0, 0));
	SetForeground(RGB(255, 255, 255));
	SetBackgroundTransparent(true);
}

GameLabel::~GameLabel()
{
}

void GameLabel::Render(HDC hdc)
{
	// 判断是否可见
	if (!m_visible)
		return;

	HFONT hFont, hOldFont;
	Util::CreateLogFont(hFont, m_h);
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	int nOldBkMode;
	COLORREF oldBkColor, oldTextColor;
	if (m_background_transparent)
	{
		// 设置背景透明
		nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
	}
	else 
	{
		// 设置背景不透明
		nOldBkMode = ::SetBkMode(hdc, OPAQUE);
		// 设置背景色
		oldBkColor = ::SetBkColor(hdc, m_background);
	}

	// 设置前景色
	oldTextColor = ::SetTextColor(hdc, m_foreground);
	// 渲染文本
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
	// 计算文本宽度
	Util::GetStringWidth(hdc, hFont, m_text, &m_w);
	::ReleaseDC(Game::GetGameHwnd(), hdc);
	SafeDeleteGDIObject(hFont);
}

void GameLabel::SetFontSize(int nHeight)
{
	// 设置字体大小
	m_h = nHeight;
	// 重新计算文本宽度
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
