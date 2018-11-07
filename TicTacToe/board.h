
//++++++++++++++++++++++++++++++++++
// ��Ϸ��ս�����
//----------------------------------

#pragma once

#ifndef __BOARD_H__
#define __BOARD_H__

#include <windows.h>
#include "prece.h"
#include "game_type.h"

class Board
{
public:
	~Board();

public:
	void Init(HWND);				// ��ʼ��
	void Render(HDC);				// ��������
	void DrawBoard(HDC);			// ����"��"
	void MouseDown(int, int);		// �������Ϣ

public:
	Prece** GetPreces() { return m_ppPreces; }	// ��ȡ�����ϵ���������

private:
	RECT m_rect;					// ���̾���
	ClickPlayer m_cur_click;		// ��ǰ���
	Prece** m_ppPreces;				// �����ϵ���������
};


#endif // !__BOARD_H__

