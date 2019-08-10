//++++++++++++++++++++++++++++++++++
// ��Ϸ��ť��
//----------------------------------

#pragma once

#ifndef __GAME_BUTTON_H__
#define __GAME_BUTTON_H__

// ö�ٰ�ť���״̬
enum ButtonMouseState {
	MOVE, LBUTTONDOWN, LBUTTONUP
};

class GameButton
{
public:
	GameButton(int x = 0, int y = 0, LPCTSTR text = NULL);
	~GameButton();

public:
	// ��Ⱦ��ť
	virtual void Render(HDC hdc);										
	// ���ĵ���¼�
	virtual void OnClick(int x, int y);									
	// ������Ϣ����
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
	// ���Ľ��밴ť�¼�
	virtual void OnMouseEnter(int x, int y);							
	// ����ͣ���¼�
	virtual void OnMouseHover(int x, int y);							
	// �����뿪��ť�¼�
	virtual void OnMouseLeave();										

	// ���ð�ť�ı�����ɫ
	void SetBackground(COLORREF backgroundColor);						
	// ���ð�ť��ǰ����ɫ
	void SetForeground(COLORREF foregroundColor);						
	// ���ð�ť���ͣ���ı�����ɫ
	void SetHoverBackground(COLORREF hoverBackgroundColor);				
	// ���ð�ť���ͣ����ǰ����ɫ
	void SetHoverForeground(COLORREF hoverForegroundColor);				
	// ���ð�ť������ı�����ɫ
	void SetMouseDownBackground(COLORREF mouseDownBackgroundColor);		
	// ���ð�ť�������ǰ����ɫ
	void SetMouseDownForeground(COLORREF mouseDownForegroundColor);		
	// ���ð�ť���ı�
	void SetText(LPCTSTR text);											
	// ���ð�ť�����ֳߴ�
	void SetFontSize(int nHeight);										
	// ���ð�ť��x����λ��(���Ͻ�)
	void SetX(int x);													
	// ���ð�ť��y����λ��(���Ͻ�)
	void SetY(int y);													
	// ���ð�ť�Ƿ�����
	void SetEnable(bool bEnable);										
	// ���ð�ť�Ƿ�ɼ�
	void SetVisible(bool bVisible);										
	// ���ð�ť�ĵ������(function)����
	void SetClickDelegate(GameButtonClickDelegateFunction fnClick);		
	// ��ȡ��ť���ı�
	LPCTSTR GetText();													
	// ��ȡ��ť�Ŀ��
	int GetWidth();														
	// ��ȡ��ť�ĸ߶�
	int GetHeight();													
	// ��ȡ��ť��x����λ��(���Ͻ�)
	int GetX();															
	// ��ȡ��ť��y����λ��(���Ͻ�)
	int GetY();															
	// �Ƿ�ɼ�
	bool IsVisible();													
	// �Ƿ�����
	bool IsEnable();													

protected:
	GameButtonClickDelegateFunction m_fn_click;	// ��ť����ĺ���(function)����
	LPCTSTR m_text;								// ��ť���ı�
	COLORREF m_background;						// ��ť�ı�����ɫ
	COLORREF m_foreground;						// ��ť��ǰ����ɫ
	COLORREF m_hover_background;				// ���ͣ��ʱ��ť�ı�����ɫ
	COLORREF m_hover_foreground;				// ���ͣ��ʱ��ť��ǰ����ɫ
	COLORREF m_mouse_down_background;			// �����ʱ�ı�����ɫ
	COLORREF m_mouse_down_foreground;			// �����ʱ��ǰ����ɫ
	int m_x;									// ��ť��x����λ��
	int m_y;									// ��ť��y����λ��
	int m_text_w;								// ��ť���ı��Ŀ��
	int m_text_h;								// ��ť���ļ��ĸ߶�
	int m_w;									// ��ť�Ŀ��
	int m_h;									// ��ť�ĸ߶�
	bool m_mouse_enter;							// ����Ƿ���밴ť
	bool m_mouse_down;							// ����Ƿ�����ť
	bool m_enable;								// ��ť�Ƿ�����
	bool m_visible;								// ��ť�Ƿ�ɼ�
};

#endif // !__GAME_BUTTON_H__

