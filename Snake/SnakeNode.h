//++++++++++++++++++++++++++++++++++
// 蛇身的节点类
//----------------------------------

#pragma once

#ifndef __SNAKE_NODE_H__
#define __SNAKE_NODE_H__

#define RENDER_BORDER false	// Debug 是否渲染贪吃蛇节点的边框，true即为渲染，false即为不渲染

class SnakeNode
{
public:
	SnakeNode(int x, int y, int width, int height, SnakeNodeType type);
	~SnakeNode();

public:
	// 渲染贪吃蛇的节点
	void Render(HDC hdc);			
	// 移动贪吃蛇的节点
	void MoveTo(int x, int y);		
	// 设置贪吃蛇节点的方向
	void SetDir(SnakeNodeDir dir);	
	// 获取贪吃蛇节点的方向
	SnakeNodeDir GetDir();			
	// 获取贪吃蛇节点的x坐标位置
	int GetX();						
	// 获取贪吃蛇节点的y坐标位置
	int GetY();						

private:
	// 渲染贪吃蛇的蛇头节点
	void RenderHead(HDC hdc);		
	// 渲染贪吃蛇的蛇尾节点
	void RenderTail(HDC hdc);		
	// 渲染贪吃蛇的蛇身节点
	void RenderBody(HDC hdc);		

private:
	SnakeNodeType m_type;			// 贪吃蛇节点的类型
	SnakeNodeDir m_dir;				// 贪吃蛇节点的方向
	int m_x;						// 贪吃蛇节点的x坐标位置
	int m_y;						// 贪吃蛇节点的y坐标位置
	int m_w;						// 贪吃蛇节点的宽度
	int m_h;						// 贪吃蛇节点的高度
};

#endif // !__SNAKE_NODE_H__
