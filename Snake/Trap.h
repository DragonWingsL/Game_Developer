//++++++++++++++++++++++++++++++++++
// 障壁类
//----------------------------------

#pragma once

#ifndef __TRAP_H__
#define __TRAP_H__

enum EnumTrapStyle;	// 声明障壁样式枚举

class Trap 
{
public:
	Trap(EnumTrapStyle style, int x, int y, int w, int h);
	~Trap();

public:
	// 食物的渲染
	void Render(HDC hdc);			

	// 返回食物的x坐标位置
	int GetX();						
	// 返回食物的y坐标位置
	int GetY();						

private:
	EnumTrapStyle m_style;	// 障壁的样式
	int m_x;				// 食物的X坐标位置
	int m_y;				// 食物的Y坐标位置
	int m_w;				// 食物的宽度
	int m_h;				// 食物的高度
};

#endif // !__TRAP_H__
