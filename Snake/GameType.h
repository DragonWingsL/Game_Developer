//++++++++++++++++++++++++++++++++++++++++
// 游戏中使用的数据类型
//----------------------------------------

#pragma once

#ifndef __GAME_TYPE_H__
#define __GAME_TYPE_H__

#include <functional>	// 提供一组函数对象类(此处我们用到了std::function函数对象)

/* 枚举蛇的组件类型 */
enum SnakeNodeType {
	SNT_HEAD,	// 贪吃蛇的头
	SNT_BODY,	// 贪吃蛇的身体
	SNT_TAIL	// 贪吃蛇的尾巴
};

/* 枚举蛇的组件方向 */
enum SnakeNodeDir {
	SND_UP,		// 上
	SND_DOWN,	// 下
	SND_LEFT,	// 左
	SND_RIGHT,	// 右
};

/* 枚举游戏使用的按键 */
enum Keys {
	KEY_W,		// W按键
	KEY_S,		// S按键
	KEY_A,		// A按键
	KEY_D,		// D按键
	KEY_UP,		// 上箭头按键
	KEY_DOWN,	// 下箭头按键
	KEY_LEFT,	// 左箭头按键
	KEY_RIGHT	// 右箭头按键
};

enum SnakeHeadCollisionType {
	SHCT_AIR,	// 空
	SHCT_SELF,	// 自身
	SHCT_TRAP,	// 障壁
	SHCT_FRUIT	// 食物
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
