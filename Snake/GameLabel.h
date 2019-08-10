//++++++++++++++++++++++++++++++++++
// 游戏标签类
//----------------------------------

#pragma once

#ifndef __GAME_LABEL_H__
#define __GAME_LABEL_H__

class GameLabel
{
public:
	GameLabel(int x = 0, int y = 0, LPCTSTR text = NULL);
	~GameLabel();

public:
	// 渲染标签
	void Render(HDC hdc);								
	// 设置标签文本
	void SetText(LPCTSTR text);							
	// 设置标签字体尺寸
	void SetFontSize(int nHeight);						
	// 设置标签x坐标位置
	void SetX(int x);									
	// 设置标签y坐标位置
	void SetY(int y);									
	// 设置标签背景颜色
	void SetBackground(COLORREF background);			
	// 设置标签前景颜色
	void SetForeground(COLORREF foreground);			
	// 设置标签背景是否透明
	void SetBackgroundTransparent(bool bTransparent);	
	// 设置标签是否启用
	void SetEnable(bool bEnable);						
	// 设置标签是否可见
	void SetVisible(bool bVisible);						
	// 获取标签文本
	LPCTSTR GetText();									
	// 获取标签x坐标位置
	int GetX();											
	// 获取标签y坐标位置
	int GetY();											
	// 获取标签宽度
	int GetWidth();										
	// 获取标签宽度
	int GetHeight();									
	// 是否可见
	bool IsVisible();									
	// 是否启用
	bool IsEnable();									

protected:
	int m_x;						// 标签x坐标位置
	int m_y;						// 标签y坐标位置	
	int m_w;						// 标签宽度
	int m_h;						// 标签高度
	bool m_background_transparent;	// 标签背景是否透明
	bool m_enable;					// 标签是否启用
	bool m_visible;					// 标签是否可见
	COLORREF m_background;			// 标签的背景颜色
	COLORREF m_foreground;			// 标签的前景颜色
	LPCTSTR m_text;					// 标签的文本
};

#endif // !__GAME_LABEL_H__
