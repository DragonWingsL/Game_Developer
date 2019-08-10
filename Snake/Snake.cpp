
#include <windows.h>		// WIN32开发中最重要的头文件(不用详解了吧...)
#include <vector>			// vector容器所在头文件

#include "GameType.h"		// 游戏中使用到的一些枚举、别名定义
#include "SnakeNode.h"		// 蛇身节点类
#include "Snake.h"			// 贪吃蛇类

Snake::Snake(int nMaxLenght) 
	: m_max_length(nMaxLenght),		// 设定蛇身的最大长度
	m_change_speed_percent(.01f),	// 设定速度改变的百分比
	m_change_speed_count(6)		// 设置速度改变根据的身长
{
}

Snake::~Snake()
{
}

void Snake::Init(int latticeWidth, int latticeHeight, int xCenter, int yCenter)
{
	// 清空蛇的节点
	m_components.clear();

	// 初始化节点宽度和高度
	m_component_width = latticeWidth;
	m_component_height = latticeHeight;
	// 设置横向和纵向每次移动的距离
	m_h_move_distance = m_component_width;
	m_v_move_distance = m_component_height;
	// 设置移动延迟
	m_move_delay = 130L;
	// 初始化距离移动的时间差
	m_update_delay = m_move_delay;
	// 设定可改变蛇头方向
	m_can_change_dir = true;

	/* *
	 * 初始化蛇的组件, 生成三个组件
	 * 分别是: 蛇头、蛇身、蛇尾
	 * */

	int pos_arr[][2] = {
		{ xCenter, yCenter },
		{ xCenter, yCenter + m_component_height },
		{ xCenter, yCenter + 2 * m_component_height }
	};

	// 获取蛇头的坐标(x, y) 并创建蛇头 
	int x_head, y_head;
	x_head = pos_arr[0][0];
	y_head = pos_arr[0][1];
	SnakeNode head(x_head, y_head, m_component_width, m_component_height, SnakeNodeType::SNT_HEAD);
	head.SetDir(SnakeNodeDir::SND_UP);
	m_components.push_back(head);

	// 创建蛇身 
	// - 2 是因为不把尾巴当蛇身进行创建
	const int c_body = ARRAYSIZE(pos_arr) - 2;
	int i_body = 0;
	int x_body, y_body;
	while (true)
	{
		if (i_body >= c_body)
			break;

		x_body = pos_arr[i_body + 1][0];
		y_body = pos_arr[i_body + 1][1];
		SnakeNode body(x_body, y_body, m_component_width, m_component_height, SnakeNodeType::SNT_BODY);
		body.SetDir(SnakeNodeDir::SND_UP);
		m_components.push_back(body);
		++i_body;
	}

	// 创建蛇尾 
	int x_tail, y_tail;
	x_tail = pos_arr[i_body + 1][0];
	y_tail = pos_arr[i_body + 1][1];
	SnakeNode tail(x_tail, y_tail, m_component_width, m_component_height, SnakeNodeType::SNT_TAIL);
	tail.SetDir(SnakeNodeDir::SND_UP);
	m_components.push_back(tail);
}

void Snake::Render(HDC hdc)
{
	// 绘制蛇的组件,通过迭代器
	std::vector<SnakeNode>::iterator iter_component;
	iter_component = m_components.begin();
	// 遍历节点
	while (true)
	{
		// 当遍历到最后一个节点
		if (iter_component == m_components.end())
			break;
		// 渲染节点
		iter_component->Render(hdc);
		++iter_component;
	}
}

void Snake::Update(DWORD dwDeltaTime)
{
	// 判断是否暂停
	if (!m_is_pause)
	{
		// 移动蛇
		Move(dwDeltaTime);
	}
}

void Snake::OnKeyDown(Keys key)
{
	// 距离上一次移动只能改变一次蛇头方向
	if (m_can_change_dir)
	{
		// 根据按键改变蛇头方向
		SnakeNode &head = *m_components.begin();
		if ((key == Keys::KEY_W || key == Keys::KEY_UP) && head.GetDir() != SnakeNodeDir::SND_DOWN)
			head.SetDir(SnakeNodeDir::SND_UP);
		else if ((key == Keys::KEY_S || key == Keys::KEY_DOWN) && head.GetDir() != SnakeNodeDir::SND_UP)
			head.SetDir(SnakeNodeDir::SND_DOWN);
		else if ((key == Keys::KEY_A || key == Keys::KEY_LEFT) && head.GetDir() != SnakeNodeDir::SND_RIGHT)
			head.SetDir(SnakeNodeDir::SND_LEFT);
		else if ((key == Keys::KEY_D || key == Keys::KEY_RIGHT) && head.GetDir() != SnakeNodeDir::SND_LEFT)
			head.SetDir(SnakeNodeDir::SND_RIGHT);
		else
			return;

		// 设置更新方向完毕
		m_can_change_dir = false;
	}
}

int Snake::GetNodeCount()
{
	// 获取节点数
	return m_components.size();
}

bool Snake::GetNodesPosition(int *&pPositionArray)
{
	if (NULL == pPositionArray)
		return false;

	// 遍历节点，将节点的x y放入数组中
	size_t i;
	for (i = 0; i < m_components.size() * 2; i += 2)
	{
		pPositionArray[i] = m_components[i/2].GetX();
		pPositionArray[i + 1] = m_components[i/2].GetY();
	}
	return (i == m_components.size() * 2);
}

int Snake::GetHeadX()
{
	// 获取蛇头x坐标
	return m_components.begin()->GetX();
}

int Snake::GetHeadY()
{
	// 获取蛇头y坐标
	return m_components.begin()->GetY();
}

SnakeNodeDir Snake::GetHeadDir()
{
	// 获取蛇头方向
	return m_components.begin()->GetDir();
}

void Snake::Move(DWORD dwDeltaTime)
{
	m_update_delay -= dwDeltaTime;

	// 与最后一次移动蛇身时间戳做比较，如果达到移动间隔时间，则移动
	if (m_update_delay > 0)
		return;

	// 设定可改变蛇头方向
	m_can_change_dir = true;

	// 移动蛇,通过反向迭代器
	std::vector<SnakeNode>::reverse_iterator rev_iter_component;
	rev_iter_component = m_components.rbegin();
	int x_body, y_body;
	while (true)
	{
		// 如果到达最后一个,则退出
		if (rev_iter_component == m_components.rend())
			break;
		// 如果为倒数第二个,则为蛇头
		if ((rev_iter_component + 1) == m_components.rend())
		{
			// 移动蛇头
			MoveHead(*rev_iter_component);
			break;
		}

		// 移动蛇的身体和尾巴,通过移动到前一个节点的位置即可完成移动
		x_body = (rev_iter_component + 1)->GetX();
		y_body = (rev_iter_component + 1)->GetY();
		rev_iter_component->MoveTo(x_body, y_body);
		rev_iter_component->SetDir((rev_iter_component + 1)->GetDir());
		++rev_iter_component;
	}

	// 保存当前更新的时间戳
	m_update_delay = m_move_delay;
}

void Snake::MoveHead(SnakeNode &head)
{
	// 移动蛇头
	int x_head, y_head;
	GetSnakeHeadNextMovePosition(&x_head, &y_head, 0);
	head.MoveTo(x_head, y_head);
}

bool Snake::IsMaxLength()
{
	// 贪吃蛇的身体是否达到最大长度
	return m_components.size() >= m_max_length;
}

void Snake::Pause()
{
	// 暂停贪吃蛇的移动
	m_is_pause = true;
}

void Snake::Continue()
{
	// 继续贪吃蛇的移动
	m_is_pause = false;
}

bool Snake::IsEatSelf(DWORD dwDeltaTime)
{
	// 判断是否咬到自身
	int x_head, y_head;
	// 获取当前蛇头移动到的位置(模拟移动)
	GetSnakeHeadNextMovePosition(&x_head, &y_head, dwDeltaTime);
	// 如果未达到移动时间, 蛇头并未发生移动, 不需要做计算, 直接返回
	if (x_head == GetHeadX() && y_head == GetHeadY())
		return false;

	// 遍历蛇的节点(从蛇头开始,因为移动后蛇头就改变了,第二个节点会移动到当前蛇头的位置,而现在蛇头位置并未改变)
	for (size_t i = 0; i < m_components.size() - 1; ++i)
	{
		// 通过判断坐标判断蛇头与蛇身的节点是否重叠,如重叠,则咬到了
		if (m_components[i].GetX() == x_head && m_components[i].GetY() == y_head)
			return true;
	}
	return false;
}

void Snake::GetSnakeHeadNextMovePosition(int *pX, int *pY, DWORD dwDeltaTime)
{
	// 获取蛇头
	SnakeNode head = *m_components.begin();
	// 通过计算当前时间戳与最后一次移动的时间戳比较, 是否达到移动时间间隔
	if (m_update_delay - (LONG)dwDeltaTime > 0)
	{
		// 未到达,返回当前蛇头位置
		*pX = head.GetX();
		*pY = head.GetY();
		return;
	}
	// 到达,则返回移动后的位置(但蛇头并未移动)

	// 前进的距离(x, y)
	int x_forward = 0;
	int y_forward = 0;

	// x
	if (head.GetDir() == SnakeNodeDir::SND_LEFT)
		x_forward = -m_h_move_distance;
	else if (head.GetDir() == SnakeNodeDir::SND_RIGHT)
		x_forward = m_h_move_distance;

	// y
	if (head.GetDir() == SnakeNodeDir::SND_UP)
		y_forward = -m_v_move_distance;
	else if (head.GetDir() == SnakeNodeDir::SND_DOWN)
		y_forward = m_v_move_distance;

	*pX = head.GetX() + x_forward;
	*pY = head.GetY() + y_forward;
}

void Snake::Increase()
{
	// 检测是否达到蛇身的最大长度
	if (IsMaxLength())
		return;

	// 获取插入的蛇身节点(蛇尾节点)
	std::vector<SnakeNode>::iterator iter_insert = m_components.begin() + (m_components.size() - 1);
	// 创建蛇身节点
	SnakeNode body(iter_insert->GetX(), iter_insert->GetY(), m_component_width, m_component_height, SnakeNodeType::SNT_BODY);
	body.SetDir(iter_insert->GetDir());
	// 将新的蛇身节点插入到尾巴节点的前面
	m_components.insert(iter_insert, body);

	// 检测蛇身的长度是否达到改变速度的数量
	if (GetNodeCount() % m_change_speed_count == 0)
	{
		m_move_delay -= (DWORD)(m_move_delay * m_change_speed_percent);
	}
}
