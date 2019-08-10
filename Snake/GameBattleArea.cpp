
#include <windows.h>			// WIN32开发中最重要的头文件(不用详解了吧...)
#include <tchar.h>				// 通用字符集头文件,对于字符集之间的兼容比如ASCII编码和Unicode编码
#include <time.h>				// 使用与时间相关的函数(例如这里的time())
#include <vector>				// vector容器所在头文件
#include <algorithm>			// 提供了大量基于迭代器的非成员模板函数

#include "GameType.h"			// 游戏中使用到的一些枚举、别名定义
#include "Snake.h"				// 贪吃蛇类
#include "Trap.h"				// 障壁类
#include "Fruit.h"				// 食物类
#include "util.h"				// 常用工具类
#include "GameBattleArea.h"		// 游戏对战场类

GameBattleArea::GameBattleArea(RECT rcBattle, const int nHLatticeCount, const int nVLatticeCount) 
	: m_battle_rect(rcBattle),				// 初始化对战的矩形
	m_h_lattice_count(nHLatticeCount),		// 初始化对战的横向格子数
	m_v_lattice_count(nVLatticeCount)		// 初始化对战的纵向格子数
{
	// 创建贪吃蛇
	m_snake = new Snake(m_v_lattice_count * m_h_lattice_count);
	// 创建食物
	m_fruit = new Fruit(1);
	// 初始化格子宽度和高度
	m_lattice_width = (m_battle_rect.right - m_battle_rect.left) / m_h_lattice_count;
	m_lattice_height = (m_battle_rect.bottom - m_battle_rect.top) / m_v_lattice_count;
	// 初始化失败的 "×" 的缩放率
	m_defeat_tips_scale_rate = .3f;
	// 初始化放大缩放
	m_is_scale_up = true;
}

GameBattleArea::~GameBattleArea()
{
	// 释放游戏游戏对战场的资源
	SafeDeleteObject(m_snake);
	SafeDeleteObject(m_fruit);

	// 清空障壁
	m_traps.clear();
}

void GameBattleArea::Init(HWND hWnd)
{
	// 清空障壁
	m_traps.clear();

	// 初始化蛇的坐标位置
	int center_x, center_y;
	center_x = m_battle_rect.left + (int)(m_h_lattice_count / 2 * m_lattice_width + m_lattice_width * .5f);
	center_y = m_battle_rect.top + (int)(m_v_lattice_count / 2 * m_lattice_height + m_lattice_height * .5f);

	// 初始化蛇
	m_snake->Init(m_lattice_width, m_lattice_height, center_x, center_y);

	// 生成水果
	RespawnFruit();

	// 生成障壁
	SpawnTraps();
}

void GameBattleArea::Render(HDC hdc)
{
	//渲染游戏游戏对战场

	HBRUSH hBattleBackgroundBrush = ::CreateSolidBrush(RGB(144, 81, 73));
	// 填充游戏对战场的背景
	::FillRect(hdc, &m_battle_rect, hBattleBackgroundBrush);

	// 渲染蛇
	m_snake->Render(hdc);

	if (m_fruit != NULL) {
		// 渲染食物
		m_fruit->Render(hdc);
	}

	// 遍历渲染障壁
	for (int i = 0, count = m_traps.size(); i < count; ++i)
		m_traps[i].Render(hdc);

	// 渲染失败提示的 "×"
	if (m_is_game_over)
		RenderDefeatTips(hdc);

	SafeDeleteGDIObject(hBattleBackgroundBrush);
}

void GameBattleArea::Update(DWORD dwDeltaTime)
{
	// 游戏对战场的更新

	// 判断是否暂停或游戏结束
	if (!m_is_pause && !m_is_game_over)
	{
		// 蛇头碰撞的类型
		SnakeHeadCollisionType collisionType = SnakeHeadCollisionType::SHCT_AIR;
		// 获取蛇头碰撞的类型
		GetSnakeHeadCollision(collisionType, dwDeltaTime);

		switch (collisionType)
		{
		case SHCT_SELF: // 咬到自己
			m_is_game_over = true;
			m_game_over_type = GameOverType::GOT_SELF;
			fnGameOver();
			break;
		case SHCT_TRAP: // 碰到障壁
			m_is_game_over = true;
			m_game_over_type = GameOverType::GOT_TRAP;
			fnGameOver();
			break;
		case SHCT_FRUIT: // 吃到食物
			// 蛇身自增
			m_snake->Increase();
			// 更新贪吃蛇
			m_snake->Update(dwDeltaTime);
			// 判断贪吃蛇是否达到最大长度
			if (!m_snake->IsMaxLength())
			{
				// 增加分数
				fnBattleScoreAdd(m_fruit->GetScore());
				// 生成食物
				RespawnFruit();
			}
			else 
			{
				// 贪吃蛇赢得了比赛
				m_is_game_over = true;
				m_game_over_type = GameOverType::GOT_WIN;
				fnGameOver();
			}
			break;
		default:
			// 更新贪吃蛇
			m_snake->Update(dwDeltaTime);
			break;
		}
	}

	// 判断是否游戏结束,进行缩放失败的 "×"
	if (m_is_game_over)
	{
		// 判断是否放大缩放
		if (m_is_scale_up)
		{
			// 按照一定比率缩放
			m_defeat_tips_scale_rate += dwDeltaTime * .001f;
			// 达到最大缩放,进行缩小
			if (m_defeat_tips_scale_rate >= 1.0f)
			{
				m_defeat_tips_scale_rate = 1.0f;
				m_is_scale_up = false;
			}
		}
		else 
		{
			// 按照一定比率缩放
			m_defeat_tips_scale_rate -= dwDeltaTime * .001f;
			// 达到最小缩放,进行放大
			if (m_defeat_tips_scale_rate <= .3f)
			{
				m_defeat_tips_scale_rate = .3f;
				m_is_scale_up = true;
			}
		}
	}
}

void GameBattleArea::OnKeyDown(Keys key)
{
	// 判断是否暂停 或 游戏结束
	if (m_is_pause || m_is_game_over)
		return;

	// 贪吃蛇按键处理
	m_snake->OnKeyDown(key);
}

void GameBattleArea::RespawnFruit()
{
	// 获取蛇身节点数，以用来确定保存蛇身的数组
	int array_count = m_snake->GetNodeCount();
	// 根据蛇身数，进行创建数组
	int *pNodesPositionArray = new int[array_count * 2];
	// 将蛇身的所有节点的位置保存到数组
	// 保存的格式为: array[0]为x坐标, array[1]为y坐标
	m_snake->GetNodesPosition(pNodesPositionArray);

	// 将位置坐标进行重新计算为格子坐标
	std::vector<int> nodes;
	int temp1 = 0, temp2 = 0;
	for (int i = 0, count = array_count * 2; i < count; i += 2)
	{
		// 将蛇的节点x坐标位置转换为格子的x坐标
		temp1 = (pNodesPositionArray[i] - m_battle_rect.left) / m_lattice_width - 1;
		// 将蛇的节点y坐标位置转换为格子的y坐标
		temp2 = (pNodesPositionArray[i + 1] - m_battle_rect.top) / m_lattice_height - 1;
		// 将格子坐标(x, y)以数值保存
		// 数值格式: x + y * (横向格子数 - 2(这个2为左右障壁)),
		// 这样做是为了方便使用索引进行删除非空格子
		// 因为empties保存的顺序就是与这个数值相关的
		nodes.push_back(temp1 + temp2 * (m_h_lattice_count-2));
	}

	// 将除了障壁的格子的坐标都保存到vector容器中
	std::vector<POINT> empties;
	for (int row = 1, row_count = m_v_lattice_count - 1; row < row_count; ++row)
	{
		for (int col = 1, col_count = m_h_lattice_count - 1; col < col_count; ++col)
		{
			// 将格子坐标以POINT结构体进行保存
			POINT pt; pt.x = col; pt.y = row;
			// 添加到empties中
			empties.push_back(pt);
		}
	}

	temp1 = temp2 = 0;
	// 将蛇的节点坐标数值以降序的方式进行排序
	std::sort(nodes.begin(), nodes.end());
	// 通过坐标的数字使用索引进行删除非空节点
	for (int i = 0, count = nodes.size(); i < count; ++i)
	{
		// temp1为empties中非空格子的索引
		// items[i]为蛇的节点所在的节点坐标(以数值的格式)
		// temp2为已在empties中删除的格子数
		temp1 = nodes[i] - temp2;
		// 使用erase进行删除指定的非空格子
		empties.erase(empties.begin() + temp1);
		// 累加删除的格子数
		++temp2;
	}

	// 如果存在空格子
	if (empties.size() > 0)
	{
		// 随机种子
		srand((unsigned)time(NULL));
		// 随机数
		int rnd = rand() % empties.size();
		// 食物的横纵坐标
		int x_fruit = empties[rnd].x;
		int y_fruit = empties[rnd].y;
		// 计算食物的位置坐标
		x_fruit = m_battle_rect.left + x_fruit * m_lattice_width + m_lattice_width / 2;
		y_fruit = m_battle_rect.top + y_fruit * m_lattice_height + m_lattice_height / 2;
		// 重新设置新的食物的位置坐标
		int score = rand() % 3;
		m_fruit->Reset(score == 0 ? 1 : score, x_fruit, y_fruit, m_lattice_width, m_lattice_height);
	}

	// 释放占用的资源
	empties.clear();
	nodes.clear();
	SafeDeleteArrayObject(pNodesPositionArray);
}

void GameBattleArea::GetSnakeHeadCollision(SnakeHeadCollisionType &collisionType, DWORD dwDeltaTime)
{
	// 蛇头碰撞类型
	collisionType = SnakeHeadCollisionType::SHCT_AIR;
	// 横纵坐标偏移量
	const int x_offset = m_battle_rect.left + (int)(m_lattice_width * .5f);
	const int y_offset = m_battle_rect.top + (int)(m_lattice_height * .5f);
	// 蛇头位置坐标
	int x_snake_head, y_snake_head;
	m_snake->GetSnakeHeadNextMovePosition(&x_snake_head, &y_snake_head, dwDeltaTime);
	// 重计算蛇头位置为格子坐标
	int x_head_lattice = (x_snake_head - x_offset) / m_lattice_width;
	int y_head_lattice = (y_snake_head - y_offset) / m_lattice_height;

	// 蛇头位置在障壁的区域内
	if (x_head_lattice == 0 || x_head_lattice == m_h_lattice_count-1 ||
		y_head_lattice == 0 || y_head_lattice == m_v_lattice_count-1)
	{
		collisionType = SnakeHeadCollisionType::SHCT_TRAP;
		return;
	}

	int x_fruit_lattice = (m_fruit->GetX() - x_offset) / m_lattice_width;
	int y_fruit_lattice = (m_fruit->GetY() - y_offset) / m_lattice_height;
	// 蛇头位置在食物的位置中
	if (x_head_lattice == x_fruit_lattice && y_head_lattice == y_fruit_lattice)
	{
		collisionType = SnakeHeadCollisionType::SHCT_FRUIT;
		return;
	}

	// 蛇头的位置在蛇身中
	if (m_snake->IsEatSelf(dwDeltaTime))
	{
		collisionType = SnakeHeadCollisionType::SHCT_SELF;
		return;
	}
}

void GameBattleArea::Pause()
{
	// 暂停对战
	m_is_pause = true;
	// 暂停贪吃蛇的移动
	m_snake->Pause();
}

void GameBattleArea::Continue()
{
	// 继续对战
	m_is_pause = false;
	// 继续贪吃蛇的移动
	m_snake->Continue();
}

void GameBattleArea::SpawnTraps()
{
	// 初始化默认障壁格式
	EnumTrapStyle trapStyle = EnumTrapStyle::TS_DOWNTIP;
	// 遍历格子
	for (int row = 0; row < m_v_lattice_count; ++row)
	{
		for (int col = 0; col < m_h_lattice_count; ++col)
		{
			// 如果格子为第一列 或者 最后一列
			if (row == 0 || row == m_v_lattice_count - 1)
			{
				// 如果为第一行 或者 最后一行 (即为四个角)
				// 第一列为x   第一行为y   即(0, 0)
				// 最后一列为x 第一行为y   即(h, 0)
				// 第一列为x   最后一行为y 即(0, v)
				// 第一列为x   最后一行为y 即(h, v)
				if (col == 0 || col == m_h_lattice_count - 1)
				{
					// 设置障壁样式为块
					trapStyle = EnumTrapStyle::TS_BLOCK;
				}
				else
				{
					// 中间部分, 根据行数
					// 行数为第一行,则为向下的刺
					// 行数为最后一行,则为向上的刺
					trapStyle = row == 0 ? EnumTrapStyle::TS_UPTIP : EnumTrapStyle::TS_DOWNTIP;
				}
			}
			// 如果为第一列
			else if (col == 0)
			{
				// 则为左边的刺
				trapStyle = EnumTrapStyle::TS_LEFTTIP;
			}
			// 如果为最后一列
			else if (col == m_h_lattice_count - 1)
			{
				// 则为右边的刺
				trapStyle = EnumTrapStyle::TS_RIGHTTIP;
			}
			else
				// 如果不是四边的格子,则跳过
				continue;

			// 创建障壁
			Trap trap(trapStyle, (int)(m_battle_rect.left + col * m_lattice_width + m_lattice_width * .5f),
				(int)(m_battle_rect.top + row * m_lattice_height + m_lattice_height * .5f), m_lattice_width, m_lattice_height);
			// 添加到保存障壁的容器中
			m_traps.push_back(trap);
		}
	}
}

bool GameBattleArea::IsGameOver()
{
	// 返回是否对战结束
	return m_is_game_over;
}

bool GameBattleArea::IsPause()
{
	return m_is_pause;
}

void GameBattleArea::Restart(HWND hWnd)
{
	// 重新开始对战
	m_is_game_over = false;
	m_is_pause = false;
	// 重新初始化对战
	Init(hWnd);
}

GameOverType GameBattleArea::GetGameOverType()
{
	// 游戏结束的类型
	return m_game_over_type;
}

void GameBattleArea::RenderDefeatTips(HDC hdc)
{
	// 判断是否胜利,则返回
	if (m_game_over_type == GameOverType::GOT_WIN)
		return;

	HPEN hPen = ::CreatePen(PS_SOLID, 6, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	// 获取蛇头位置坐标
	int x_center = m_snake->GetHeadX();
	int y_center = m_snake->GetHeadY();

	// 根据蛇头位置,进行加上对应的偏移量,则为碰撞到的位置
#if false
	// 凑合着看吧...o((⊙﹏⊙))o
	"
	 * 
	 *       \   / <----- 失败的提示
	 *       *****  
	 *      **/ \**   
	 *     **     **  
	 *    **       ** 
	 *   *************
	 *   *************
	 *   *************
	 *
	 "
#endif 
	switch (m_snake->GetHeadDir())
	{
	case SND_UP:
		y_center -= (int)(m_lattice_height * .5f);
		break;
	case SND_DOWN:
		y_center += (int)(m_lattice_height * .5f);
		break;
	case SND_LEFT:
		x_center -= (int)(m_lattice_width * .5f);
		break;
	case SND_RIGHT:
		x_center += (int)(m_lattice_width * .5f);
		break;
	default:
		break;
	}

	/* 绘制 "×" */
	float len = m_lattice_width * .5f * m_defeat_tips_scale_rate;
	float angles[] = {
		45, 135, 225, 315
	};

	int points[2][4];

	float rad = 3.1415926f / 180.0f;
	/* 第一条 */
	int x_lt = (int)(x_center + len * cos(angles[0] * rad));
	int y_lt = (int)(y_center + len * sin(angles[0] * rad));
	int x_rd = (int)(x_center + len * cos(angles[2] * rad));
	int y_rd = (int)(y_center + len * sin(angles[2] * rad));

	/* 第二条 */
	int x_rt = (int)(x_center + len * cos(angles[1] * rad));
	int y_rt = (int)(y_center + len * sin(angles[1] * rad));
	int x_ld = (int)(x_center + len * cos(angles[3] * rad));
	int y_ld = (int)(y_center + len * sin(angles[3] * rad));

	points[0][0] = x_lt;
	points[0][1] = y_lt;
	points[0][2] = x_rd;
	points[0][3] = y_rd;

	points[1][0] = x_rt;
	points[1][1] = y_rt;
	points[1][2] = x_ld;
	points[1][3] = y_ld;

	/* 绘制 */
	Util::DrawLine(hdc, points[0]);
	Util::DrawLine(hdc, points[1]);

	::SelectObject(hdc, hOldPen);
	SafeDeleteGDIObject(hPen);
}

