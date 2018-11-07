
#include "board.h"

Board::~Board()
{
	// �ͷžŸ����Ӷ���
	if (m_ppPreces != NULL) {
		delete m_ppPreces;
		m_ppPreces = NULL;
	}
}

void Board::Init(HWND hWnd)
{
	// �趨���1Ϊ��ʼ���
	m_cur_click = ClickPlayer::ClickPlayer_Player1;
	::GetClientRect(hWnd, &m_rect);

	/* ��ʼ���Ÿ����� m_ppPreces��ΪNULL��Ϊ�Ͼ����� */
	if (m_ppPreces != NULL) {
		delete m_ppPreces;
		m_ppPreces = NULL;
	}
	m_ppPreces = new Prece*[9];

	/* �������� */
	int x_margin = 20;
	int y_margin = 20;

	/* �趨���̾��δ�С */
	m_rect.left += x_margin;
	m_rect.top += y_margin;
	m_rect.right -= x_margin;
	m_rect.bottom -= y_margin;

	/* ���̿�� */
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	/* �������Ͻ�(x, y) �Լ����ӵĿ�͸� */
	int x_start = m_rect.left;
	int y_start = m_rect.top;
	int w_distance = width / 3;
	int h_distance = height / 3;

	for (int c = 0, col = 3; c < col; ++c)
	{
		for (int r = 0, row = 3; r < row; ++r)
		{
			/* �������̸���,�����浽������ */
			m_ppPreces[c * 3 + r] = new Prece(x_start + (r * w_distance), y_start + (c * h_distance), w_distance, h_distance, c * 3 + r);
		}
	}
}

void Board::Render(HDC hdc)
{
	/* ����"��" */
	DrawBoard(hdc);

	/* ���������� */
	for (int i = 0, count = 9; i < 9; ++i)
	{
		m_ppPreces[i]->Render(hdc);
	}
}

void Board::MouseDown(int x, int y)
{
	/* ����Ƿ��������� */
	for (int i = 0, count = 9; i < 9; ++i)
	{
		if (m_ppPreces[i]->CheckClick(x, y))
		{
			/* �趨���ӱ���ǰ������ҵ�� */
			m_ppPreces[i]->Click(m_cur_click);
			/* ���������,���л�������� */
			m_cur_click = (m_cur_click == ClickPlayer::ClickPlayer_Player1 ? 
				ClickPlayer::ClickPlayer_Player2 : 
				ClickPlayer::ClickPlayer_Player1);
		}
	}
}

void Board::DrawBoard(HDC hdc)
{
	/* �������� */
	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	/* ���̿�� */
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;
	int x_left_top = m_rect.left;
	int y_left_top = m_rect.top;
	int w_distance = width / 3;
	int h_distance = height / 3;

	/* "��"�ı�� */
	int points[4][4];

	/* ���ߵ�һ�� */
	points[0][0] = x_left_top + w_distance;
	points[0][1] = y_left_top;
	points[0][2] = x_left_top + w_distance;
	points[0][3] = y_left_top + height;

	/* ���ߵڶ��� */
	points[1][0] = x_left_top + 2 * w_distance;
	points[1][1] = y_left_top;
	points[1][2] = x_left_top + 2 * w_distance;
	points[1][3] = y_left_top + height;

	/* ���ߵ�һ�� */
	points[2][0] = x_left_top;
	points[2][1] = y_left_top + h_distance;
	points[2][2] = x_left_top + width;
	points[2][3] = y_left_top + h_distance;

	/* ���ߵڶ��� */
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
