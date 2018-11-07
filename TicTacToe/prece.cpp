
#include "prece.h"

Prece::Prece(int x, int y, int w, int h, int index)
{
	/* 格子初始化 */
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_index = index;
	m_bIsClick = false;
}

void Prece::Render(HDC hdc)
{
	/* 绘制标记 */
	DrawGraphics(hdc);
}

bool Prece::CheckClick(int x, int y)
{
	/* 判断鼠标点击的位置是否在格子内 */
	return (!m_bIsClick) && (x <= m_x + m_w && y <= m_y + m_h && x >= m_x && y >= m_y);
}

void Prece::Click(ClickPlayer sender)
{
	/* 格子被点击 */
	m_bIsClick = true;
	/* 设定点击玩家 */
	m_click_player = sender;
}

void Prece::DrawGraphics(HDC hdc)
{
	/* 判断棋子是否被玩家点击 */
	if (!m_bIsClick)
		return;

	if (m_click_player == ClickPlayer::ClickPlayer_Player1)
	{
		/* 绘制玩家1图形 */
		DrawPlayer1Graphics(hdc);
	}
	else
	{
		/* 绘制玩家2图形 */
		DrawPlayer2Graphics(hdc);
	}
}

void Prece::DrawPlayer1Graphics(HDC hdc)
{
	// 棋子中心点坐标
	int x_center = m_x + (m_w / 2);
	int y_center = m_y + (m_h / 2);

	/* 绘制 "×" */
	double len = m_w / 3.0;
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

	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(153, 77, 82));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	/* 绘制 */
	Util::DrawLine(hdc, points[0]);
	Util::DrawLine(hdc, points[1]);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
}

void Prece::DrawPlayer2Graphics(HDC hdc)
{
	/* 棋子中心点坐标 */
	int x_center = m_x + (m_w / 2);
	int y_center = m_y + (m_h / 2);

	/* "○"半径 */
	int r = m_w / 3;
	/* 绘制 "○" */
	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(64, 116, 52));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	HBRUSH bOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	::Ellipse(hdc, x_center - r, y_center - r, x_center + r, y_center + r);

	::SelectObject(hdc, bOldBrush);
	::SelectObject(hdc, hOldPen);
	::DeleteObject(hBrush); hBrush = NULL;
	::DeleteObject(hPen); hPen = NULL;
}