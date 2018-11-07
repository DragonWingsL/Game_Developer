//++++++++++++++++++++++++++++++++++
// 游戏格子类
//----------------------------------

#pragma once

#ifndef __PRECE_H__
#define __PRECE_H__

#include <windows.h>
#include <cmath>

#include "game_type.h"
#include "util.h"

class Prece
{
public:
	Prece(int, int, int, int, int);

public:
	void Render(HDC);					// 绘制棋子
	void DrawGraphics(HDC);				// 绘制棋子点击后的图形,比如玩家1的是"×",玩家2的是"○"
	void DrawPlayer1Graphics(HDC);		// 绘制玩家1点击图形
	void DrawPlayer2Graphics(HDC);		// 绘制玩家2点击图形
	bool CheckClick(int, int);			// 当鼠标点击时候,判断鼠标是否点击到棋子
	void Click(ClickPlayer);			// 棋子被点击

public:
	bool IsClick() { return m_bIsClick; };						// 棋子是否被点击
	ClickPlayer GetClickPlayer() { return m_click_player; }		// 获取点击该棋子的玩家
	int GetIndex() { return m_index; }							// 获取棋子的下标

private:
	int m_index;						// 格子的下标
	int m_x;							// 棋子横坐标
	int m_y;							// 棋子纵坐标
	int m_w;							// 棋子的宽度
	int m_h;							// 棋子的高度
	bool m_bIsClick;					// 棋子是否被点击
	ClickPlayer m_click_player;			// 点击棋子的玩家
};

#endif // !__PRECE_H__


