
#include "game.h"

void Game::Init(HWND hWnd)
{
	/* 游戏初始化 */
	m_bIsGameOver = false;
	m_hWnd = hWnd;
	/* 初始化棋盘 */
	m_board.Init(hWnd);
}

void Game::Render(HDC hdc)
{
	/* 绘制游戏背景 */
	DrawBackground(hdc);
	/* 绘制棋盘 */
	m_board.Render(hdc);
	/* 绘制游戏结束 */
	DrawGameOver(hdc);
}

void Game::MouseDown(int x, int y)
{
	if (m_bIsGameOver)
	{
		/* 如果游戏结束,点击重新开始 */
		Init(m_hWnd);
		return;
	}
	/* 游戏鼠标点击消息处理 */
	m_board.MouseDown(x, y);
	/* 检测是否游戏结束 */
	CheckGameOver();
}

void Game::CheckGameOver()
{
	/* 获取棋盘格子 */
	Prece** ppPreces = m_board.GetPreces();

	/* 获取玩家点击了的格子 */
	int p1[9];
	int p2[9];
	memset((void*)p1, -1, sizeof(p1));
	memset((void*)p2, -1, sizeof(p2));

	int index1 = 0;
	int index2 = 0;
	for (int i = 0, count = 9; i < count; ++i)
	{
		if (ppPreces[i] != NULL && ppPreces[i]->IsClick())
		{
			ppPreces[i]->GetClickPlayer() == ClickPlayer::ClickPlayer_Player1 ?
				p1[index1++] = ppPreces[i]->GetIndex() :
				p2[index2++] = ppPreces[i]->GetIndex();
		}
	}

	/* 不足3个取消比较 */
	if (index1 < 3 && index2 < 3)
		return;
	
	/* 8种获胜结果集合 */
	int win_set[8][3] = {
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},

		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},

		{0, 4, 8},
		{2, 4, 6}
	};

	/* 进行比较 */
	int nP1Match = 0;
	int nP2Match = 0;
	for (int i = 0; i < 8; ++i)
	{
		nP1Match = 0;
		nP2Match = 0;
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < index1; ++k)
			{
				if (p1[k] == win_set[i][j])
					++nP1Match;
				else if (p2[k] == win_set[i][j])
					++nP2Match;

				if (nP1Match == 3)
				{
					m_over_type = GameOverType::GameOverType_Player1;
					m_bIsGameOver = true;
					return;
				}
				else if (nP2Match == 3)
				{
					m_over_type = GameOverType::GameOverType_Player2;
					m_bIsGameOver = true;
					return;
				}
			}
		}
	}

	/* 9个为平局 */
	if (index1 + index2 >= 9)
	{
		m_over_type = GameOverType::GameOverType_Tie;
		m_bIsGameOver = true;
	}

}

void Game::DrawGameOver(HDC hdc)
{
	/* 绘制游戏结束信息 */
	if (m_bIsGameOver)
	{
		LPCWSTR lpszTitle = _T("游戏结束");
		LPCWSTR lpszBody = NULL;
		LPCWSTR lpszTips = _T("点击屏幕重新开始游戏");

		/* 设置显示消息 */
		if (m_over_type == GameOverType::GameOverType_Tie)
			lpszBody = _T("  平局");
		else if (m_over_type == GameOverType::GameOverType_Player1)
			lpszBody = _T("玩家1获胜");
		else
			lpszBody = _T("玩家2获胜");

		// 设置绘制的文字字体
		HFONT hFont, hOldFont;
		Util::CreateLogFont(hFont, 45);
		hOldFont = (HFONT)SelectObject(hdc, hFont);

		/* 文字背景为透明 */
		::SetBkMode(hdc, TRANSPARENT);

		/* 绘制标题 */
		::SetTextColor(hdc, RGB(197, 31, 31));
		::TextOut(hdc, 150, 100, lpszTitle, lstrlen(lpszTitle));

		/* 绘制信息 */
		::SetTextColor(hdc, RGB(87, 105, 60));
		::TextOut(hdc, 150, 225, lpszBody, lstrlen(lpszBody));

		/* 绘制提示消息 */
		::SetTextColor(hdc, RGB(91, 74, 66));
		::TextOut(hdc, 0, 350, lpszTips, lstrlen(lpszTips));

		::SelectObject(hdc, hOldFont);
		::DeleteObject(hFont); hFont = NULL;
	}
}

void Game::DrawBackground(HDC hdc)
{
	/* 创建背景颜色画刷 */
	HBRUSH brush = ::CreateSolidBrush(RGB(22, 22, 22));
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	/* 填充背景颜色 */
	::FillRect(hdc, &rect, brush);
	::DeleteObject(brush); brush = NULL;
}
