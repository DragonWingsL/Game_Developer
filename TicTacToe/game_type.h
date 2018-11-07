//++++++++++++++++++++++++++++++++++
// ��Ϸ�ж��������
//----------------------------------

#pragma once

#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

/* ������ö�� */
typedef enum _ClickPlayer {
	ClickPlayer_Player1 = 0x00000001,	// ���1
	ClickPlayer_Player2 = 0x00000002	// ���2
}ClickPlayer;

/* ��Ϸ��� */
typedef enum _GameOverType {
	GameOverType_Tie	 = 0x00000001,	// ƽ��
	GameOverType_Player1 = 0x00000002,	// ���1
	GameOverType_Player2 = 0x00000004	// ���2
}GameOverType;

#endif // !__GAME_TYPE_H__