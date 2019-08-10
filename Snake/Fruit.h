//++++++++++++++++++++++++++++++++++
// 食物类
//----------------------------------

#pragma once

#ifndef __FRUIT_H__
#define __FRUIT_H__

class Fruit
{
public:
	Fruit(int score) : m_score(score), m_x(0), m_y(0), m_w(0), m_h(0) { }
	Fruit(int score, int x, int y, int w, int h);
	~Fruit();

public:
	// 食物的渲染
	void Render(HDC hdc);					
	// 重新设置食物的位置和宽度
	void Reset(int score, int x, int y, int w, int h);	
	// 返回食物的x坐标位置
	int GetX();								
	// 返回食物的y坐标位置
	int GetY();								
	// 获取食物的分数
	int GetScore();							

private:
	int m_score;	// 食物的分数
	int m_x;		// 食物的x坐标位置
	int m_y;		// 食物的y坐标位置
	int m_w;		// 食物的宽度
	int m_h;		// 食物的高度
};

#endif // !__FRUIT_H__
