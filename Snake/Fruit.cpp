
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)

#include "util.h"			// 常用工具类
#include "Fruit.h"			// 食物类

Fruit::Fruit(int score, int x, int y, int width, int height) 
	: m_score(score),	// 初始化食物的分数
	m_x(x),				// 初始化食物的x坐标位置
	m_y(y),				// 初始化食物的y坐标位置
	m_w(width),			// 初始化食物的宽度
	m_h(height)			// 初始化食物的高度
{
}

Fruit::~Fruit()
{
}

void Fruit::Render(HDC hdc)
{
	// 绘制食物

	// 创建空画笔
	HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(NULL_PEN));
	// 创建画刷,根据分数创建不同的颜色
	HBRUSH hBrush = ::CreateSolidBrush(GetScore() > 1 ? RGB(255, 0, 0) : RGB(0, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// 绘制食物 "○"
	::Ellipse(hdc, (int)(m_x - m_w * .5f), (int)(m_y - m_h * .5f), (int)(m_x + m_w * .5f), (int)(m_y + m_h * .5f));

	::SelectObject(hdc, hOldPen);
	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hBrush);
}

int Fruit::GetX()
{
	// 获取食物的x坐标位置
	return m_x;
}

int Fruit::GetY()
{
	// 获取食物的y坐标位置
	return m_y;
}

void Fruit::Reset(int score, int x, int y, int w, int h)
{
	// 重设食物的位置宽度以及分数
	m_score = score;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

int Fruit::GetScore()
{
	// 获取食物的分数
	return m_score;
}
