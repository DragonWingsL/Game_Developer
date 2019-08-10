//++++++++++++++++++++++++++++++++++++++++
// ��Ϸ��ʹ�õ���������
//----------------------------------------

#pragma once

#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include <functional>	// �ṩһ�麯��������(�˴������õ���std::function��������)

/* ö���ߵ�������� */
enum SnakeNodeType {
	SNT_HEAD,	// ̰���ߵ�ͷ
	SNT_BODY,	// ̰���ߵ�����
	SNT_TAIL	// ̰���ߵ�β��
};

/* ö���ߵ�������� */
enum SnakeNodeDir {
	SND_UP,		// ��
	SND_DOWN,	// ��
	SND_LEFT,	// ��
	SND_RIGHT,	// ��
};

/* ö����Ϸʹ�õİ��� */
enum Keys {
	KEY_W,		// W����
	KEY_S,		// S����
	KEY_A,		// A����
	KEY_D,		// D����
	KEY_UP,		// �ϼ�ͷ����
	KEY_DOWN,	// �¼�ͷ����
	KEY_LEFT,	// ���ͷ����
	KEY_RIGHT	// �Ҽ�ͷ����
};

enum SnakeHeadCollisionType {
	SHCT_AIR,	// ��
	SHCT_SELF,	// ����
	SHCT_TRAP,	// �ϱ�
	SHCT_FRUIT	// ʳ��
};

enum GameOverType {
	GOT_SELF,
	GOT_TRAP,
	GOT_WIN
};

enum EnumTrapStyle {
	TS_BLOCK,
	TS_UPTIP,
	TS_DOWNTIP,
	TS_LEFTTIP,
	TS_RIGHTTIP
};

typedef std::function<void(void)> GameDelegateFunction;
typedef std::function<void(int, int)> GameButtonClickDelegateFunction;
typedef std::function<void(int)> GameBattleScoreDelegateFunction;

#endif // !__GAME_TYPE_H__
