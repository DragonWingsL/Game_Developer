//++++++++++++++++++++++++++++++++++
// ��Ϸ������
//----------------------------------

#pragma once

#ifndef __PRECE_H__
#define __PRECE_H__

#include <windows.h>
#include <cmath>

#include "game_type.h"
#include "util.h"

class Prece
{
public:
	Prece(int, int, int, int, int);

public:
	void Render(HDC);					// ��������
	void DrawGraphics(HDC);				// �������ӵ�����ͼ��,�������1����"��",���2����"��"
	void DrawPlayer1Graphics(HDC);		// �������1���ͼ��
	void DrawPlayer2Graphics(HDC);		// �������2���ͼ��
	bool CheckClick(int, int);			// �������ʱ��,�ж�����Ƿ���������
	void Click(ClickPlayer);			// ���ӱ����

public:
	bool IsClick() { return m_bIsClick; };						// �����Ƿ񱻵��
	ClickPlayer GetClickPlayer() { return m_click_player; }		// ��ȡ��������ӵ����
	int GetIndex() { return m_index; }							// ��ȡ���ӵ��±�

private:
	int m_index;						// ���ӵ��±�
	int m_x;							// ���Ӻ�����
	int m_y;							// ����������
	int m_w;							// ���ӵĿ��
	int m_h;							// ���ӵĸ߶�
	bool m_bIsClick;					// �����Ƿ񱻵��
	ClickPlayer m_click_player;			// ������ӵ����
};

#endif // !__PRECE_H__


