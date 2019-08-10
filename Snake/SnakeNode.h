//++++++++++++++++++++++++++++++++++
// ����Ľڵ���
//----------------------------------

#pragma once

#ifndef __SNAKE_NODE_H__
#define __SNAKE_NODE_H__

#define RENDER_BORDER false	// Debug �Ƿ���Ⱦ̰���߽ڵ�ı߿�true��Ϊ��Ⱦ��false��Ϊ����Ⱦ

class SnakeNode
{
public:
	SnakeNode(int x, int y, int width, int height, SnakeNodeType type);
	~SnakeNode();

public:
	// ��Ⱦ̰���ߵĽڵ�
	void Render(HDC hdc);			
	// �ƶ�̰���ߵĽڵ�
	void MoveTo(int x, int y);		
	// ����̰���߽ڵ�ķ���
	void SetDir(SnakeNodeDir dir);	
	// ��ȡ̰���߽ڵ�ķ���
	SnakeNodeDir GetDir();			
	// ��ȡ̰���߽ڵ��x����λ��
	int GetX();						
	// ��ȡ̰���߽ڵ��y����λ��
	int GetY();						

private:
	// ��Ⱦ̰���ߵ���ͷ�ڵ�
	void RenderHead(HDC hdc);		
	// ��Ⱦ̰���ߵ���β�ڵ�
	void RenderTail(HDC hdc);		
	// ��Ⱦ̰���ߵ�����ڵ�
	void RenderBody(HDC hdc);		

private:
	SnakeNodeType m_type;			// ̰���߽ڵ������
	SnakeNodeDir m_dir;				// ̰���߽ڵ�ķ���
	int m_x;						// ̰���߽ڵ��x����λ��
	int m_y;						// ̰���߽ڵ��y����λ��
	int m_w;						// ̰���߽ڵ�Ŀ��
	int m_h;						// ̰���߽ڵ�ĸ߶�
};

#endif // !__SNAKE_NODE_H__
