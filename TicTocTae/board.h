
//++++++++++++++++++++++++++++++++++
// 游戏对战面板类
//----------------------------------

#pragma once

#ifndef __BOARD_H__
#define __BOARD_H__

#include <windows.h>
#include "prece.h"
#include "game_type.h"

class Board
{
public:
	~Board();

public:
	void Init(HWND);				// 初始化
	void Render(HDC);				// 绘制棋盘
	void DrawBoard(HDC);			// 绘制"井"
	void MouseDown(int, int);		// 鼠标点击消息

public:
	Prece** GetPreces() { return m_ppPreces; }	// 获取棋盘上的所有棋子

private:
	RECT m_rect;					// 棋盘矩形
	ClickPlayer m_cur_click;		// 当前玩家
	Prece** m_ppPreces;				// 棋盘上的所有棋子
};


#endif // !__BOARD_H__

