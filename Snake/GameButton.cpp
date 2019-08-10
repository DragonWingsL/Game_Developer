
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)
#include <windowsx.h>		// 包含的大量有用的API宏等(这里用到了GET_X_LPARAM和GET_Y_LPARAM)
#include <tchar.h>			// 通用字符集头文件,对于字符集之间的兼容比如ASCII编码和Unicode编码

#include "Util.h"			// 常用工具类
#include "Game.h"			// 游戏类
#include "GameType.h"		// 游戏中使用到的一些枚举、别名定义
#include "GameButton.h"		// 游戏按钮类

GameButton::GameButton(int x, int y, LPCTSTR text) 
	: m_x(x),			// 初始化按钮的x坐标位置
	m_y(y),				// 初始化按钮的y坐标位置
	m_visible(true),	// 初始化按钮的可见性
	m_enable(true)		// 初始化按钮的启用
{
	// 初始化背景色和前景色
	SetBackground(RGB(255, 24, 84));
	SetForeground(RGB(255, 255, 255));
	SetHoverBackground(RGB(255, 36, 96));
	SetHoverForeground(RGB(255, 255, 255));
	SetMouseDownBackground(RGB(255, 48, 108));
	SetMouseDownForeground(RGB(255, 255, 255));
	// 初始化默认字体大小
	SetFontSize(100);
	// 初始化默认文本
	SetText(text == NULL ? _T("GameButton") : text);
}

GameButton::~GameButton()
{
}

void GameButton::Render(HDC hdc)
{
	// 渲染按钮

	// 判断是否可见,则返回
	if (!m_visible)
		return;

	// 背景颜色
	COLORREF background = m_background, foreground = m_foreground;
	if (m_mouse_down)
	{
		// 点击的背景和前景颜色
		foreground = m_mouse_down_foreground;
		background = m_mouse_down_background;
	}
	else if (m_mouse_enter)
	{
		// 鼠标在按钮中的背景和前景颜色
		foreground = m_hover_foreground;
		background = m_hover_background;
	}

	HBRUSH hBrush = ::CreateSolidBrush(background);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// 绘制按钮背景

	// 路径开始
	::BeginPath(hdc);
	// 绘制一个圆角矩形
	::RoundRect(hdc, m_x, m_y, m_x + m_w, m_y + m_h, 25, 50);
	// 路径结束
	::EndPath(hdc);
	// 填充路径.
	// 注意: FillPath结束后选择的路径会自动清除
	::FillPath(hdc);

	HFONT hFont, hOldFont;
	Util::CreateLogFont(hFont, m_text_h);
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	// 设置字体背景模式(透明或不透明)
	int nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
	// 设置字体背景颜色
	COLORREF hOldTextColor = ::SetTextColor(hdc, foreground);

	// 绘制按钮文字
	::TextOut(hdc, m_x + 5, m_y + 5, m_text, wcslen(m_text));

	::SetTextColor(hdc, hOldTextColor);
	::SetBkMode(hdc, nOldBkMode);
	::SelectObject(hdc, hOldFont);
	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hBrush);
	SafeDeleteGDIObject(hFont);
}

void GameButton::OnClick(int x, int y)
{
	// 调用点击委托函数
	m_fn_click(x, y);
}

void GameButton::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 按钮的窗口消息处理

	// 如果不可见和不启用,则返回
	if (!m_enable || !m_visible)
		return;

	int x, y;
	// 鼠标在按钮的状态
	ButtonMouseState mouseState = ButtonMouseState::MOVE;
	switch (message)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		// 左键按下
		mouseState = ButtonMouseState::LBUTTONDOWN;
		break;
	case WM_LBUTTONUP:
		// 左键松开
		mouseState = ButtonMouseState::LBUTTONUP;
		break;
	default:
		return;
	}

	// 获取鼠标位置
	x = GET_X_LPARAM(lParam);
	y = GET_Y_LPARAM(lParam);

	// 检测鼠标坐标是否在按钮内
	if (Util::CheckRectangleContainsPoint(m_x, m_y, m_x + m_w, m_y + m_h, x, y))
	{
		// 鼠标移动
		if (mouseState == ButtonMouseState::MOVE)
		{
			// 判断鼠标是否已经进入按钮
			if (m_mouse_enter)
			{
				// 调用鼠标在按钮停留的事件
				this->OnMouseHover(x, y);
			}
			else
			{
				// 设置鼠标进入
				this->m_mouse_enter = true;
				// 调用鼠标进入按钮的事件
				this->OnMouseEnter(x, y);
			}
		}
		else if (mouseState == ButtonMouseState::LBUTTONDOWN) // 鼠标左键点下
		{
			// 设置鼠标左键点下
			this->m_mouse_down = true;
		}
		else  // 鼠标松开
		{
			// 设置鼠标左键点击取消
			if (this->m_mouse_down)
				this->OnClick(x, y); // 调用点击事件
		}
	}
	else 
	{
		// 鼠标移动离开按钮
		if (m_mouse_enter)
		{
			// 设置鼠标离开
			this->m_mouse_enter = false;
			// 调用鼠标离开按钮事件
			this->OnMouseLeave();
		}
	}

	// 鼠标松开
	if (mouseState == ButtonMouseState::LBUTTONUP)
	{
		this->m_mouse_down = false;
	}
}

void GameButton::OnMouseEnter(int x, int y)
{
}

void GameButton::OnMouseHover(int x, int y)
{
}

void GameButton::OnMouseLeave()
{
}

void GameButton::SetBackground(COLORREF backgroundColor)
{
	m_background = backgroundColor;
}

void GameButton::SetForeground(COLORREF foregroundColor)
{
	m_foreground = foregroundColor;
}

void GameButton::SetHoverBackground(COLORREF hoverBackgroundColor)
{
	m_hover_background = hoverBackgroundColor;
}

void GameButton::SetHoverForeground(COLORREF hoverForegroundColor)
{
	m_hover_foreground = hoverForegroundColor;
}

void GameButton::SetMouseDownBackground(COLORREF mouseDownBackgroundColor)
{
	m_mouse_down_background = mouseDownBackgroundColor;
}

void GameButton::SetMouseDownForeground(COLORREF mouseDownForegroundColor)
{
	m_mouse_down_foreground = mouseDownForegroundColor;
}

void GameButton::SetText(LPCTSTR text)
{
	// 设置按钮文字
	if (text == NULL)
	{
		text = _T("");
	}
	m_text = text;
	// 获取窗口DC
	HDC hdc = ::GetDC(Game::GetGameHwnd());
	HFONT hFont;
	Util::CreateLogFont(hFont, m_text_h);
	// 获取文本宽度
	Util::GetStringWidth(hdc, hFont, m_text, &m_text_w);
	SafeDeleteGDIObject(hFont);
	m_w = m_text_w + 2 * 5;
}

void GameButton::SetFontSize(int nHeight)
{
	// 设置文本高度
	m_text_h = nHeight;
	// 设置按钮高度
	m_h = m_text_h + 2 * 5;
	// 重设置文本,计算宽度
	SetText(GetText());
}

void GameButton::SetClickDelegate(GameButtonClickDelegateFunction fnClick)
{
	// 设置按钮点击函数(function)对象
	m_fn_click = fnClick;
}

LPCTSTR GameButton::GetText()
{
	// 获取文本
	return m_text;
}

int GameButton::GetWidth()
{
	// 获取宽度
	return m_w;
}

int GameButton::GetHeight()
{
	// 获取高度
	return m_h;
}

int GameButton::GetX()
{
	// 获取按钮x坐标位置
	return m_x;
}

int GameButton::GetY()
{
	// 获取按钮y坐标位置
	return m_y;
}

bool GameButton::IsVisible()
{
	return m_visible;
}

bool GameButton::IsEnable()
{
	return m_enable;
}

void GameButton::SetX(int x)
{
	// 设置按钮x坐标位置
	m_x = x;
}

void GameButton::SetY(int y)
{
	// 设置按钮y坐标位置
	m_y = y;
}

void GameButton::SetEnable(bool bEnable)
{
	// 设置按钮释放启用
	m_enable = bEnable;
}

void GameButton::SetVisible(bool bVisible)
{
	// 设置按钮是否可见
	m_visible = bVisible;
}
