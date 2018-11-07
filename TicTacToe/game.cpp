
#include "game.h"

void Game::Init(HWND hWnd)
{
	/* ��Ϸ��ʼ�� */
	m_bIsGameOver = false;
	m_hWnd = hWnd;
	/* ��ʼ������ */
	m_board.Init(hWnd);
}

void Game::Render(HDC hdc)
{
	/* ������Ϸ���� */
	DrawBackground(hdc);
	/* �������� */
	m_board.Render(hdc);
	/* ������Ϸ���� */
	DrawGameOver(hdc);
}

void Game::MouseDown(int x, int y)
{
	if (m_bIsGameOver)
	{
		/* �����Ϸ����,������¿�ʼ */
		Init(m_hWnd);
		return;
	}
	/* ��Ϸ�������Ϣ���� */
	m_board.MouseDown(x, y);
	/* ����Ƿ���Ϸ���� */
	CheckGameOver();
}

void Game::CheckGameOver()
{
	/* ��ȡ���̸��� */
	Prece** ppPreces = m_board.GetPreces();

	/* ��ȡ��ҵ���˵ĸ��� */
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

	/* ����3��ȡ���Ƚ� */
	if (index1 < 3 && index2 < 3)
		return;
	
	/* 8�ֻ�ʤ������� */
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

	/* ���бȽ� */
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

	/* 9��Ϊƽ�� */
	if (index1 + index2 >= 9)
	{
		m_over_type = GameOverType::GameOverType_Tie;
		m_bIsGameOver = true;
	}

}

void Game::DrawGameOver(HDC hdc)
{
	/* ������Ϸ������Ϣ */
	if (m_bIsGameOver)
	{
		LPCWSTR lpszTitle = _T("��Ϸ����");
		LPCWSTR lpszBody = NULL;
		LPCWSTR lpszTips = _T("�����Ļ���¿�ʼ��Ϸ");

		/* ������ʾ��Ϣ */
		if (m_over_type == GameOverType::GameOverType_Tie)
			lpszBody = _T("  ƽ��");
		else if (m_over_type == GameOverType::GameOverType_Player1)
			lpszBody = _T("���1��ʤ");
		else
			lpszBody = _T("���2��ʤ");

		// ���û��Ƶ���������
		HFONT hFont, hOldFont;
		Util::CreateLogFont(hFont, 45);
		hOldFont = (HFONT)SelectObject(hdc, hFont);

		/* ���ֱ���Ϊ͸�� */
		::SetBkMode(hdc, TRANSPARENT);

		/* ���Ʊ��� */
		::SetTextColor(hdc, RGB(197, 31, 31));
		::TextOut(hdc, 150, 100, lpszTitle, lstrlen(lpszTitle));

		/* ������Ϣ */
		::SetTextColor(hdc, RGB(87, 105, 60));
		::TextOut(hdc, 150, 225, lpszBody, lstrlen(lpszBody));

		/* ������ʾ��Ϣ */
		::SetTextColor(hdc, RGB(91, 74, 66));
		::TextOut(hdc, 0, 350, lpszTips, lstrlen(lpszTips));

		::SelectObject(hdc, hOldFont);
		::DeleteObject(hFont); hFont = NULL;
	}
}

void Game::DrawBackground(HDC hdc)
{
	/* ����������ɫ��ˢ */
	HBRUSH brush = ::CreateSolidBrush(RGB(22, 22, 22));
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	/* ��䱳����ɫ */
	::FillRect(hdc, &rect, brush);
	::DeleteObject(brush); brush = NULL;
}
