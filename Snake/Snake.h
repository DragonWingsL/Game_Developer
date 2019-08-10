//++++++++++++++++++++++++++++++++++
// 贪吃蛇类
//----------------------------------

#pragma once

#ifndef __SNAKE_H__
#define __SNAKE_H__

class SnakeNode;			// 声明贪吃蛇的节点类
enum Keys;					// 声明按键的枚举

class Snake
{
public:
	Snake(int nMaxLenght);
	~Snake();

public:
	// 初始化贪吃蛇
	void Init(int latticeWidth, int latticeHeight, int xCenter, int yCenter);	
	// 渲染贪吃蛇
	void Render(HDC hdc);									
	// 更新贪吃蛇
	void Update(DWORD dwDeltaTime);							
	// 按键处理
	void OnKeyDown(Keys key);								

	// 移动贪吃蛇
	void Move(DWORD dwDeltaTime);							
	// 贪吃蛇自增
	void Increase();										
	// 暂停移动贪吃蛇
	void Pause();											
	// 继续移动贪吃蛇
	void Continue();										
	// 获取贪吃蛇的蛇头下次移动到的位置
	void GetSnakeHeadNextMovePosition(int *pX, int *pY, DWORD dwDeltaTime);	
	// 获取贪吃蛇的节点数
	int GetNodeCount();										
	// 获取蛇头的x坐标位置
	int GetHeadX();											
	// 获取蛇头的y坐标位置
	int GetHeadY();											
	// 获取蛇头方向
	SnakeNodeDir GetHeadDir();								
	// 获取贪吃蛇的位置
	bool GetNodesPosition(int *&pPositionArray);			
	// 是否咬到自身
	bool IsEatSelf(DWORD dwDeltaTime);										
	// 贪吃蛇的身体是否达到最大长度
	bool IsMaxLength();										

private:
	// 移动贪吃蛇的蛇头
	void MoveHead(SnakeNode &head);							

private:
	std::vector<SnakeNode> m_components;	// 蛇的节点
	DWORD m_move_delay;						// 蛇每次移动的时间差(毫秒数)
	LONG m_update_delay;					// 蛇最后一次更新的时间戳
	float m_change_speed_percent;			// 修改移动速度的比率
	int m_change_speed_count;				// 设定蛇身达到一定程度后，修改移动速度
	int m_h_move_distance;					// 蛇头每次移动的横向距离
	int m_v_move_distance;					// 蛇头每次移动的纵向距离
	int m_component_width;					// 蛇的节点宽度
	int m_component_height;					// 蛇的节点高度
	size_t m_max_length;						// 蛇身的最大长度
	bool m_is_pause;						// 是否暂停蛇的移动
	bool m_can_change_dir;					// 此次按键是否可以改变蛇头的方向
};

#endif // !__SNAKE_H__
