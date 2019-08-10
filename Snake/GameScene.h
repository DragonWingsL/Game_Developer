//++++++++++++++++++++++++++++++++++
// 游戏场景类
//----------------------------------

#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

class GameButton;		// 声明游戏按钮类
class GameBattleArea;	// 声明游戏游戏对战场类
class GameLabel;		// 声明游戏标签类

// 继承于游戏场景类
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene() override;

public:
	// 初始化场景
	virtual void Init(HWND hWnd) override;						
	// 渲染场景
	virtual void Render(HDC hdc) override;						
	// 更新场景
	virtual void Update(DWORD dwDeltaTime) override;			
	// 场景按键处理
	virtual void OnKeyDown(Keys key) override;					
	// 窗口消息的处理
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	// 游戏暂停按钮点击事件
	void OnPuaseGameButtonClicked(int x, int y);	
	// 退出游戏按钮点击事件
	void OnExitGameButtonClicked(int x, int y);	
	// 重新开始游戏按钮点击事件
	void OnNewGameButtonClicked(int x, int y);	
	// 切换到主菜单按钮点击事件
	void OnSwitchMainMenuButtonClicked(int x, int y);	
	// 游戏结束事件
	void OnGameOver();				
	// 分数增加事件
	void OnGameScoreAdd(int score);	

private:
	// 绘制游戏结束
	void RenderGameOver(HDC hdc);	

public:
	GameDelegateFunction fnExitGame;			// 退出游戏的函数(function)对象
	GameDelegateFunction fnSwitchToMainMenu;	// 切换到主菜单的函数(function)对象

private:
	HWND m_hWnd;								// 游戏窗口句柄
	GameBattleArea *m_battle_area;				// 游戏游戏对战场
	GameButton *m_pause_button;					// 游戏暂停按钮
	GameButton *m_new_game_button;				// 重新开始按钮
	GameButton *m_switch_to_main_menu_button;	// 切换到主菜单按钮
	GameButton *m_exit_button;					// 退出游戏按钮
	GameLabel *m_score_tips_title;				// 分数标题
	GameLabel *m_score_tips;					// 分数
	GameLabel *m_score_add_tips;				// 分数增加提示
	GameLabel *m_game_time_tips_title;			// 游戏时间标题
	GameLabel *m_game_time_tips;				// 游戏时间
	GameLabel *m_game_over_title_tips;			// 游戏结束标题
	GameLabel *m_game_over_reason_tips;			// 游戏结束原因
	LPTSTR m_score_string;						// 分数的字符串缓冲区
	LPTSTR m_score_add_string;					// 分数增加提示的字符串缓冲区
	LPTSTR m_game_time_string;					// 游戏时间的字符串缓冲区
	DWORD m_game_tick;							// 游戏时间(毫秒数)
	RECT m_rect;								// 场景区域矩形
	RECT m_game_over_panel_rect;				// 游戏结束面板的矩形
	const LONG m_score_add_tips_visible_time;	// 分数增加可见时间
	LONG m_score_add_tips_time;					// 分数增加距离隐藏的时间
	int m_scene_width;							// 场景宽度
	int m_scene_height;							// 场景高度
	int m_game_over_panel_width;				// 游戏结束面板的宽度
	int m_game_over_panel_height;				// 游戏结束面板的高度
	int m_game_over_x_origin;					// 游戏结束面板的中心点x坐标
	int m_game_over_y_origin;					// 游戏结束面板的中心点y左边
	int m_score;								// 游戏分数
};

#endif // !__GAME_SCENE_H__
 