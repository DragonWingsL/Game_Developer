
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)
#include <cmath>			// 数学库头文件,包含了许多常用的数学函数

#include "GameType.h"		// 游戏中使用到的一些枚举、别名定义
#include "util.h"			// 常用工具类
#include "SnakeNode.h"		// 蛇的节点类

SnakeNode::SnakeNode(int x, int y, int width, int height, SnakeNodeType type) 
	: m_x(x),		// 初始化蛇的节点x坐标位置
	m_y(y),			// 初始化蛇的节点y坐标位置
	m_w(width),		// 初始化蛇的节点宽度
	m_h(height),	// 初始化蛇的节点高度
	m_type(type)	// 初始化蛇的节点类型
{
}


SnakeNode::~SnakeNode()
{
}

void SnakeNode::Render(HDC hdc)
{
	/* *
	 * 根据不同部位进行不同的渲染
	 * */
	
	if (m_type == SnakeNodeType::SNT_HEAD)
	{
		// 渲染蛇头节点
		RenderHead(hdc);
	}
	else if (m_type == SnakeNodeType::SNT_TAIL)
	{
		// 渲染蛇尾节点
		RenderTail(hdc);
	}
	else
	{
		// 渲染蛇身节点
		RenderBody(hdc);
	}

#pragma region 渲染边框
#if RENDER_BORDER
	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	::Rectangle(hdc, m_x - m_w / 2, m_y - m_h / 2, m_x + m_w / 2, m_y + m_h / 2);

	::SelectObject(hdc, hOldPen);
	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hPen);
#endif 
#pragma endregion
}

void SnakeNode::MoveTo(int x, int y)
{
	// 移动到左边位置
	m_x = x;
	m_y = y;
}

void SnakeNode::SetDir(SnakeNodeDir dir)
{
	// 设置节点方向
	m_dir = dir;
}

SnakeNodeDir SnakeNode::GetDir()
{
	// 返回节点方向
	return m_dir;
}

int SnakeNode::GetX()
{
	// 返回x坐标
	return m_x;
}

int SnakeNode::GetY()
{
	// 返回y坐标
	return m_y;
}

void SnakeNode::RenderHead(HDC hdc)
{
	// 绘制头部
	// 计算r,用以绘制半圆
	static const int r = (int)(m_w * .5f);
	// 节点的中心点坐标位置
	const int x_up_center = m_x;
	const int y_up_center = m_y;

	// 下半部分的矩形
	RECT rc_down;
	// 上半部分的半圆起点角度
	float start_angle = .0f;
	// 根据四个方向进行计算(都为固定的啦,用笔和纸画一下就能知道了)
	if (m_dir == SnakeNodeDir::SND_UP)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y;
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y + (int)round(m_h * .5f);
	}
	else if (m_dir == SnakeNodeDir::SND_DOWN)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y;
		start_angle = 180.0f;
	}
	else if (m_dir == SnakeNodeDir::SND_LEFT)
	{
		rc_down.left = m_x;
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y + (int)round(m_h * .5f);
		start_angle = 90.0f;
	}
	else if (m_dir == SnakeNodeDir::SND_RIGHT)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x;
		rc_down.bottom = m_y + (int)round(m_h * .5f);
		start_angle = 270.0f;
	}

	// 进行绘制
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 105, 180));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// 绘制上半部分的路径
	::BeginPath(hdc);
	::MoveToEx(hdc, x_up_center, y_up_center, NULL);
	::AngleArc(hdc, x_up_center, y_up_center, r, start_angle, 180.0f);
	::EndPath(hdc);

	// 填充上半部分
	::FillPath(hdc);
	// 绘制下半部分
	::FillRect(hdc, &rc_down, hBrush);

	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hBrush);
}

void SnakeNode::RenderTail(HDC hdc)
{
	// 尾巴绘制三角形
	const int x_center = m_x;
	const int y_center = m_y;
	static const int r = (int)round(Util::GetDistanceBetweenTwoPoint(x_center - m_w * .5f, y_center - m_h * .5f, x_center, y_center));
	static const int r2 = (int)round(m_w * .5f);

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
	if (m_dir == SnakeNodeDir::SND_UP)
		angle_index = 0;
	else if (m_dir == SnakeNodeDir::SND_DOWN)
		angle_index = 1;
	else if (m_dir == SnakeNodeDir::SND_LEFT)
		angle_index = 2;
	else if (m_dir == SnakeNodeDir::SND_RIGHT)
		angle_index = 3;

	// 根据当前尾巴方向进行绘制 尾巴"△"
	POINT points[3];
	points[0].x = x_center - (int)round((cos(angles[angle_index][0] * Util::RAD)) * r);
	points[0].y = y_center - (int)round((sin(angles[angle_index][0] * Util::RAD)) * r);

	points[1].x = x_center - (int)round((cos(angles[angle_index][1] * Util::RAD)) * r2);
	points[1].y = y_center - (int)round((sin(angles[angle_index][1] * Util::RAD)) * r2);

	points[2].x = x_center - (int)round((cos(angles[angle_index][2] * Util::RAD)) * r);
	points[2].y = y_center - (int)round((sin(angles[angle_index][2] * Util::RAD)) * r);

	// 调用Util的多边形填充
	Util::PolyFill(hdc, RGB(255, 192, 203), points, 3);
}

void SnakeNode::RenderBody(HDC hdc)
{
	// 蛇身矩形
	RECT rc_body;
	rc_body.left	= m_x - (int)round(m_w * .5f);
	rc_body.top		= m_y - (int)round(m_h * .5f);
	rc_body.right	= m_x + (int)round(m_w * .5f);
	rc_body.bottom	= m_y + (int)round(m_h * .5f);

	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 155, 190));
	// 绘制蛇身
	::FillRect(hdc, &rc_body, hBrush);
	SafeDeleteGDIObject(hBrush);
}