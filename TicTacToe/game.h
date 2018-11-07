
#pragma once

//++++++++++++++++++++++++++++++++++
// ��Ϸ��
//----------------------------------

#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#define UNICODE						// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����
#define _UNICODE					// ʹ��UNICODE����,����ڱ�����������ʹ��UNICODE�ַ����˴�����

#include <windows.h>
#include <tchar.h>

#include "board.h"

class Game
{
public:
	void Init(HWND);				// ��Ϸ��ʼ��
	void Render(HDC);				// ������Ϸ����
	void DrawGameOver(HDC);			// ������Ϸ��������
	void DrawBackground(HDC);		// ���Ʊ���
	void CheckGameOver();			// ����Ƿ���Ϸ����
	void MouseDown(int, int);		// �������Ϣ

private:
	Board m_board;					// ��Ϸ����
	bool m_bIsGameOver;				// �Ƿ������Ϸ
	GameOverType m_over_type;		// ��Ϸ���
	HWND m_hWnd;					// ���ھ��
};


#endif // !__GAME_H__
