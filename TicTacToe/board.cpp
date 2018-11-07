
#include "board.h"

Board::~Board()
{
	// 释放九个格子对象
	if (m_ppPreces != NULL) {
		delete m_ppPreces;
		m_ppPreces = NULL;
	}
}

void Board::Init(HWND hWnd)
{
	// 设定玩家1为开始玩家
	m_cur_click = ClickPlayer::ClickPlayer_Player1;
	::GetClientRect(hWnd, &m_rect);

	/* 初始化九个格子 m_ppPreces不为NULL则为上局棋子 */
	if (m_ppPreces != NULL) {
		delete m_ppPreces;
		m_ppPreces = NULL;
	}
	m_ppPreces = new Prece*[9];

	/* 两边余量 */
	int x_margin = 20;
	int y_margin = 20;

	/* 设定棋盘矩形大小 */
	m_rect.left += x_margin;
	m_rect.top += y_margin;
	m_rect.right -= x_margin;
	m_rect.bottom -= y_margin;

	/* 棋盘宽高 */
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	/* 棋盘左上角(x, y) 以及棋子的宽和高 */
	int x_start = m_rect.left;
	int y_start = m_rect.top;
	int w_distance = width / 3;
	int h_distance = height / 3;

	for (int c = 0, col = 3; c < col; ++c)
	{
		for (int r = 0, row = 3; r < row; ++r)
		{
			/* 创建棋盘格子,并保存到棋盘中 */
			m_ppPreces[c * 3 + r] = new Prece(x_start + (r * w_distance), y_start + (c * h_distance), w_distance, h_distance, c * 3 + r);
		}
	}
}

void Board::Render(HDC hdc)
{
	/* 绘制"井" */
	DrawBoard(hdc);

	/* 绘制棋子子 */
	for (int i = 0, count = 9; i < 9; ++i)
	{
		m_ppPreces[i]->Render(hdc);
	}
}

void Board::MouseDown(int x, int y)
{
	/* 检测是否点击到格子 */
	for (int i = 0, count = 9; i < 9; ++i)
	{
		if (m_ppPreces[i]->CheckClick(x, y))
		{
			/* 设定棋子被当前落棋玩家点击 */
			m_ppPreces[i]->Click(m_cur_click);
			/* 点击到格子,则切换玩家下棋 */
			m_cur_click = (m_cur_click == ClickPlayer::ClickPlayer_Player1 ? 
				ClickPlayer::ClickPlayer_Player2 : 
				ClickPlayer::ClickPlayer_Player1);
		}
	}
}

void Board::DrawBoard(HDC hdc)
{
	/* 创建画笔 */
	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	/* 棋盘宽高 */
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;
	int x_left_top = m_rect.left;
	int y_left_top = m_rect.top;
	int w_distance = width / 3;
	int h_distance = height / 3;

	/* "井"四标边 */
	int points[4][4];

	/* 竖线第一条 */
	points[0][0] = x_left_top + w_distance;
	points[0][1] = y_left_top;
	points[0][2] = x_left_top + w_distance;
	points[0][3] = y_left_top + height;

	/* 竖线第二条 */
	points[1][0] = x_left_top + 2 * w_distance;
	points[1][1] = y_left_top;
	points[1][2] = x_left_top + 2 * w_distance;
	points[1][3] = y_left_top + height;

	/* 横线第一条 */
	points[2][0] = x_left_top;
	points[2][1] = y_left_top + h_distance;
	points[2][2] = x_left_top + width;
	points[2][3] = y_left_top + h_distance;

	/* 横线第二条 */
	points[3][0] = x_left_top;
	points[3][1] = y_left_top + 2 * h_distance;
	points[3][2] = x_left_top + width;
	points[3][3] = y_left_top + 2 * h_distance;

	Util::DrawLine(hdc, points[0]);
	Util::DrawLine(hdc, points[1]);
	Util::DrawLine(hdc, points[2]);
	Util::DrawLine(hdc, points[3]);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
}
