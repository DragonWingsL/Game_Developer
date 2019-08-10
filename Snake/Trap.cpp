
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)

#include "GameType.h"		// 游戏中使用到的一些枚举、别名定义
#include "util.h"			// 常用工具类
#include "Trap.h"			// 游戏障壁类

Trap::Trap(EnumTrapStyle style, int x, int y, int width, int height) 
	: m_style(style),	// 初始化障壁的样式
	m_x(x),				// 初始化障壁的x坐标位置
	m_y(y),				// 初始化障壁的y坐标位置
	m_w(width),			// 初始化障壁的宽度
	m_h(height)			// 初始化障壁的高度
{
}

Trap::~Trap()
{
}

void Trap::Render(HDC hdc)
{
	// 根据障壁的样式进行不同的渲染
	if (m_style == EnumTrapStyle::TS_BLOCK)
	{
		// 块状障壁(四个角落的)
		// 障壁矩形
		RECT rc;
		rc.left = m_x - (int)(m_w * .5f);
		rc.top = m_y - (int)(m_h * .5f);
		rc.right = m_x + (int)(m_w * .5f);
		rc.bottom = m_y + (int)(m_h * .5f);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(115, 67, 56));
		// 绘制障壁
		::FillRect(hdc, &rc, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	else 
	{
		// 绘制三角形障壁
		// 障壁的中心点坐标位置
		const int x_center = m_x;
		const int y_center = m_y;
		static const int r = (int)round(Util::GetDistanceBetweenTwoPoint(x_center - m_w * .5f, y_center - m_h * .5f, x_center, y_center));
		static const int r2 = (int)(m_w * .5f);

		/* *
		 * 三角形的三个点
		 * 四个方向,分别为四个角度
		 * */
		static const int angles[4][3] = {
			{45,  270, 135},/* up    */
			{315, 90,  225},/* down  */
			{45,  180, 315},/* left  */
			{135, 0,   225} /* rigth */
		};

		// 根据当前角度选择上面定义的角度数组
		int angle_index;
		if (m_style == EnumTrapStyle::TS_UPTIP)
			angle_index = 0;
		else if (m_style == EnumTrapStyle::TS_DOWNTIP)
			angle_index = 1;
		else if (m_style == EnumTrapStyle::TS_LEFTTIP)
			angle_index = 2;
		else if (m_style == EnumTrapStyle::TS_RIGHTTIP)
			angle_index = 3;

		// 根据当前障壁的样式进行绘制 "◁△▽▷"
		POINT points[3];
		points[0].x = x_center - (int)round((cos(angles[angle_index][0] * Util::RAD)) * r);
		points[0].y = y_center - (int)round((sin(angles[angle_index][0] * Util::RAD)) * r);

		points[1].x = x_center - (int)round((cos(angles[angle_index][1] * Util::RAD)) * r2);
		points[1].y = y_center - (int)round((sin(angles[angle_index][1] * Util::RAD)) * r2);

		points[2].x = x_center - (int)round((cos(angles[angle_index][2] * Util::RAD)) * r);
		points[2].y = y_center - (int)round((sin(angles[angle_index][2] * Util::RAD)) * r);

		// 调用Util的多边形填充
		Util::PolyFill(hdc, RGB(115, 67, 56), points, 3);
	}
}

int Trap::GetX()
{
	// 获取障壁的x坐标位置
	return m_x;
}

int Trap::GetY()
{
	// 获取障壁的y坐标位置
	return m_y;
}
