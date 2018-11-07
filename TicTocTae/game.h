
#pragma once

//++++++++++++++++++++++++++++++++++
// 游戏类
//----------------------------------

#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#define UNICODE						// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免
#define _UNICODE					// 使用UNICODE编码,如果在编译器设置了使用UNICODE字符集此处可免

#include <windows.h>
#include <tchar.h>

#include "board.h"

class Game
{
public:
	void Init(HWND);				// 游戏初始化
	void Render(HDC);				// 绘制游戏对象
	void DrawGameOver(HDC);			// 绘制游戏结束标题
	void DrawBackground(HDC);		// 绘制背景
	void CheckGameOver();			// 检测是否游戏结束
	void MouseDown(int, int);		// 鼠标点击消息

private:
	Board m_board;					// 游戏棋盘
	bool m_bIsGameOver;				// 是否结束游戏
	GameOverType m_over_type;		// 游戏结局
	HWND m_hWnd;					// 窗口句柄
};


#endif // !__GAME_H__
