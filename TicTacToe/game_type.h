//++++++++++++++++++++++++++++++++++
// 游戏中定义的类型
//----------------------------------

#pragma once

#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

/* 点击玩家枚举 */
typedef enum _ClickPlayer {
	ClickPlayer_Player1 = 0x00000001,	// 玩家1
	ClickPlayer_Player2 = 0x00000002	// 玩家2
}ClickPlayer;

/* 游戏结局 */
typedef enum _GameOverType {
	GameOverType_Tie	 = 0x00000001,	// 平局
	GameOverType_Player1 = 0x00000002,	// 玩家1
	GameOverType_Player2 = 0x00000004	// 玩家2
}GameOverType;

#endif // !__GAME_TYPE_H__