
#include <windows.h>		// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <windowsx.h>		// �����Ĵ������õ�API���(�����õ���GET_X_LPARAM��GET_Y_LPARAM)
#include <tchar.h>			// ͨ���ַ���ͷ�ļ�,�����ַ���֮��ļ��ݱ���ASCII�����Unicode����

#include "Util.h"			// ���ù�����
#include "Game.h"			// ��Ϸ��
#include "GameType.h"		// ��Ϸ��ʹ�õ���һЩö�١���������
#include "GameButton.h"		// ��Ϸ��ť��

GameButton::GameButton(int x, int y, LPCTSTR text) 
	: m_x(x),			// ��ʼ����ť��x����λ��
	m_y(y),				// ��ʼ����ť��y����λ��
	m_visible(true),	// ��ʼ����ť�Ŀɼ���
	m_enable(true)		// ��ʼ����ť������
{
	// ��ʼ������ɫ��ǰ��ɫ
	SetBackground(RGB(255, 24, 84));
	SetForeground(RGB(255, 255, 255));
	SetHoverBackground(RGB(255, 36, 96));
	SetHoverForeground(RGB(255, 255, 255));
	SetMouseDownBackground(RGB(255, 48, 108));
	SetMouseDownForeground(RGB(255, 255, 255));
	// ��ʼ��Ĭ�������С
	SetFontSize(100);
	// ��ʼ��Ĭ���ı�
	SetText(text == NULL ? _T("GameButton") : text);
}

GameButton::~GameButton()
{
}

void GameButton::Render(HDC hdc)
{
	// ��Ⱦ��ť

	// �ж��Ƿ�ɼ�,�򷵻�
	if (!m_visible)
		return;

	// ������ɫ
	COLORREF background = m_background, foreground = m_foreground;
	if (m_mouse_down)
	{
		// ����ı�����ǰ����ɫ
		foreground = m_mouse_down_foreground;
		background = m_mouse_down_background;
	}
	else if (m_mouse_enter)
	{
		// ����ڰ�ť�еı�����ǰ����ɫ
		foreground = m_hover_foreground;
		background = m_hover_background;
	}

	HBRUSH hBrush = ::CreateSolidBrush(background);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// ���ư�ť����

	// ·����ʼ
	::BeginPath(hdc);
	// ����һ��Բ�Ǿ���
	::RoundRect(hdc, m_x, m_y, m_x + m_w, m_y + m_h, 25, 50);
	// ·������
	::EndPath(hdc);
	// ���·��.
	// ע��: FillPath������ѡ���·�����Զ����
	::FillPath(hdc);

	HFONT hFont, hOldFont;
	Util::CreateLogFont(hFont, m_text_h);
	hOldFont = (HFONT)::SelectObject(hdc, hFont);

	// �������屳��ģʽ(͸����͸��)
	int nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
	// �������屳����ɫ
	COLORREF hOldTextColor = ::SetTextColor(hdc, foreground);

	// ���ư�ť����
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
	// ���õ��ί�к���
	m_fn_click(x, y);
}

void GameButton::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��ť�Ĵ�����Ϣ����

	// ������ɼ��Ͳ�����,�򷵻�
	if (!m_enable || !m_visible)
		return;

	int x, y;
	// ����ڰ�ť��״̬
	ButtonMouseState mouseState = ButtonMouseState::MOVE;
	switch (message)
	{
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		// �������
		mouseState = ButtonMouseState::LBUTTONDOWN;
		break;
	case WM_LBUTTONUP:
		// ����ɿ�
		mouseState = ButtonMouseState::LBUTTONUP;
		break;
	default:
		return;
	}

	// ��ȡ���λ��
	x = GET_X_LPARAM(lParam);
	y = GET_Y_LPARAM(lParam);

	// �����������Ƿ��ڰ�ť��
	if (Util::CheckRectangleContainsPoint(m_x, m_y, m_x + m_w, m_y + m_h, x, y))
	{
		// ����ƶ�
		if (mouseState == ButtonMouseState::MOVE)
		{
			// �ж�����Ƿ��Ѿ����밴ť
			if (m_mouse_enter)
			{
				// ��������ڰ�ťͣ�����¼�
				this->OnMouseHover(x, y);
			}
			else
			{
				// ����������
				this->m_mouse_enter = true;
				// ���������밴ť���¼�
				this->OnMouseEnter(x, y);
			}
		}
		else if (mouseState == ButtonMouseState::LBUTTONDOWN) // ����������
		{
			// ��������������
			this->m_mouse_down = true;
		}
		else  // ����ɿ�
		{
			// �������������ȡ��
			if (this->m_mouse_down)
				this->OnClick(x, y); // ���õ���¼�
		}
	}
	else 
	{
		// ����ƶ��뿪��ť
		if (m_mouse_enter)
		{
			// ��������뿪
			this->m_mouse_enter = false;
			// ��������뿪��ť�¼�
			this->OnMouseLeave();
		}
	}

	// ����ɿ�
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
	// ���ð�ť����
	if (text == NULL)
	{
		text = _T("");
	}
	m_text = text;
	// ��ȡ����DC
	HDC hdc = ::GetDC(Game::GetGameHwnd());
	HFONT hFont;
	Util::CreateLogFont(hFont, m_text_h);
	// ��ȡ�ı����
	Util::GetStringWidth(hdc, hFont, m_text, &m_text_w);
	SafeDeleteGDIObject(hFont);
	m_w = m_text_w + 2 * 5;
}

void GameButton::SetFontSize(int nHeight)
{
	// �����ı��߶�
	m_text_h = nHeight;
	// ���ð�ť�߶�
	m_h = m_text_h + 2 * 5;
	// �������ı�,������
	SetText(GetText());
}

void GameButton::SetClickDelegate(GameButtonClickDelegateFunction fnClick)
{
	// ���ð�ť�������(function)����
	m_fn_click = fnClick;
}

LPCTSTR GameButton::GetText()
{
	// ��ȡ�ı�
	return m_text;
}

int GameButton::GetWidth()
{
	// ��ȡ���
	return m_w;
}

int GameButton::GetHeight()
{
	// ��ȡ�߶�
	return m_h;
}

int GameButton::GetX()
{
	// ��ȡ��ťx����λ��
	return m_x;
}

int GameButton::GetY()
{
	// ��ȡ��ťy����λ��
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
	// ���ð�ťx����λ��
	m_x = x;
}

void GameButton::SetY(int y)
{
	// ���ð�ťy����λ��
	m_y = y;
}

void GameButton::SetEnable(bool bEnable)
{
	// ���ð�ť�ͷ�����
	m_enable = bEnable;
}

void GameButton::SetVisible(bool bVisible)
{
	// ���ð�ť�Ƿ�ɼ�
	m_visible = bVisible;
}
