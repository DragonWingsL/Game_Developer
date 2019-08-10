//++++++++++++++++++++++++++++++++++
// 游戏游戏对战场类
//----------------------------------

#pragma once

#ifndef __GAME_BATTLE_AREA_H__
#define __GAME_BATTLE_AREA_H__

class Snake;					// 声明贪吃蛇类
class Fruit;					// 声明食物类
class Trap;						// 声明障壁类
enum SnakeHeadCollisionType;	// 声明蛇头碰撞枚举

class GameBattleArea
{
public:
	GameBattleArea(RECT rcBattle, const int nHLatticeCount, const int nVLatticeCount);
	~GameBattleArea();

public:
	// 初始化场景
	void Init(HWND hWnd);					
	// 渲染场景
	void Render(HDC hdc);					
	// 更新场景
	void Update(DWORD dwDeltaTime);			
	// 场景按键处理
	void OnKeyDown(Keys key);				

	// 检测蛇头的碰撞
	void GetSnakeHeadCollision(SnakeHeadCollisionType &collisionType, DWORD dwDeltaTime);	
	// 对战暂停
	void Pause();							
	// 对战继续
	void Continue();						
	// 游戏游戏对战场重新开始
	void Restart(HWND hWnd);				
	// 获取游戏结束的类型
	GameOverType GetGameOverType();			
	// 游戏游戏对战场是否游戏结束
	bool IsGameOver();						
	// 是否暂停游戏
	bool IsPause();							

private:
	// 生成食物
	void RespawnFruit();					
	// 生成障壁
	void SpawnTraps();						
	// 渲染失败提示
	void RenderDefeatTips(HDC hdc);			

public:
	GameDelegateFunction fnGameOver;					// 游戏结束的函数(function)对象
	GameBattleScoreDelegateFunction fnBattleScoreAdd;	// 对战分数增加的函数(function)对象

private:
	std::vector<Trap> m_traps;		// 区域中的障壁
	Snake *m_snake;					// 贪吃蛇
	Fruit *m_fruit;					// 区域中的食物
	RECT m_battle_rect;				// 区域矩形
	GameOverType m_game_over_type;	// 游戏结束类型
	const int m_h_lattice_count;	// 区域中的横向格子数
	const int m_v_lattice_count;	// 区域中的纵向格子数
	int m_lattice_width;			// 区域中的格子宽度
	int m_lattice_height;			// 区域中的格子高度
	bool m_is_pause;				// 是否暂停对战
	bool m_is_game_over;			// 是否对战结束
	bool m_is_scale_up;				// 放大或者缩写失败提示的"×"
	float m_defeat_tips_scale_rate; // 失败提示的"×"缩放比例
};

#endif // !__GAME_BATTLE_AREA_H__