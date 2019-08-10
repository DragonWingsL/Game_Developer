//++++++++++++++++++++++++++++++++++
// 游戏按钮类
//----------------------------------

#pragma once

#ifndef __GAME_BUTTON_H__
#define __GAME_BUTTON_H__

// 枚举按钮鼠标状态
enum ButtonMouseState {
	MOVE, LBUTTONDOWN, LBUTTONUP
};

class GameButton
{
public:
	GameButton(int x = 0, int y = 0, LPCTSTR text = NULL);
	~GameButton();

public:
	// 渲染按钮
	virtual void Render(HDC hdc);										
	// 鼠标的点击事件
	virtual void OnClick(int x, int y);									
	// 窗口消息处理
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
	// 鼠标的进入按钮事件
	virtual void OnMouseEnter(int x, int y);							
	// 鼠标的停留事件
	virtual void OnMouseHover(int x, int y);							
	// 鼠标的离开按钮事件
	virtual void OnMouseLeave();										

	// 设置按钮的背景颜色
	void SetBackground(COLORREF backgroundColor);						
	// 设置按钮的前景颜色
	void SetForeground(COLORREF foregroundColor);						
	// 设置按钮鼠标停留的背景颜色
	void SetHoverBackground(COLORREF hoverBackgroundColor);				
	// 设置按钮鼠标停留的前景颜色
	void SetHoverForeground(COLORREF hoverForegroundColor);				
	// 设置按钮鼠标点击的背景颜色
	void SetMouseDownBackground(COLORREF mouseDownBackgroundColor);		
	// 设置按钮鼠标点击的前景颜色
	void SetMouseDownForeground(COLORREF mouseDownForegroundColor);		
	// 设置按钮的文本
	void SetText(LPCTSTR text);											
	// 设置按钮的文字尺寸
	void SetFontSize(int nHeight);										
	// 设置按钮的x坐标位置(左上角)
	void SetX(int x);													
	// 设置按钮的y坐标位置(左上角)
	void SetY(int y);													
	// 设置按钮是否启用
	void SetEnable(bool bEnable);										
	// 设置按钮是否可见
	void SetVisible(bool bVisible);										
	// 设置按钮的点击函数(function)对象
	void SetClickDelegate(GameButtonClickDelegateFunction fnClick);		
	// 获取按钮的文本
	LPCTSTR GetText();													
	// 获取按钮的宽度
	int GetWidth();														
	// 获取按钮的高度
	int GetHeight();													
	// 获取按钮的x坐标位置(左上角)
	int GetX();															
	// 获取按钮的y坐标位置(左上角)
	int GetY();															
	// 是否可见
	bool IsVisible();													
	// 是否启用
	bool IsEnable();													

protected:
	GameButtonClickDelegateFunction m_fn_click;	// 按钮点击的函数(function)对象
	LPCTSTR m_text;								// 按钮的文本
	COLORREF m_background;						// 按钮的背景颜色
	COLORREF m_foreground;						// 按钮的前景颜色
	COLORREF m_hover_background;				// 鼠标停留时按钮的背景颜色
	COLORREF m_hover_foreground;				// 鼠标停留时按钮的前景颜色
	COLORREF m_mouse_down_background;			// 鼠标点击时的背景颜色
	COLORREF m_mouse_down_foreground;			// 鼠标点击时的前景颜色
	int m_x;									// 按钮的x坐标位置
	int m_y;									// 按钮的y坐标位置
	int m_text_w;								// 按钮中文本的宽度
	int m_text_h;								// 按钮中文件的高度
	int m_w;									// 按钮的宽度
	int m_h;									// 按钮的高度
	bool m_mouse_enter;							// 鼠标是否进入按钮
	bool m_mouse_down;							// 鼠标是否点击按钮
	bool m_enable;								// 按钮是否启用
	bool m_visible;								// 按钮是否可见
};

#endif // !__GAME_BUTTON_H__

