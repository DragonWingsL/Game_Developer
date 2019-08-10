//++++++++++++++++++++++++++++++++++
// 游戏主菜单场景类
//----------------------------------

#pragma once

#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

class Game;			// 声明游戏类
class GameButton;	// 声明游戏按钮类
class GameLabel;	// 声明游戏标签类

// 继承于游戏场景类
class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() override;

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

	// 游戏开始按钮点击事件
	void OnStartGameButtonClicked(int x, int y);				
	// 退出游戏按钮点击事件
	void OnExitGameButtonClicked(int x, int y);					

public:
	GameDelegateFunction fnExitGame;		// 退出游戏的函数(function)对象
	GameDelegateFunction fnSwitchToGame;	// 切换到游戏场景的函数(function)对象

private:
	GameButton *m_start_game_button;		// 开始游戏按钮
	GameButton *m_exit_game_button;			// 退出游戏按钮
	GameLabel *m_title_first_word_label;	// 贪吃蛇中的"贪"字标签
	GameLabel *m_title_second_word_label;	// 贪吃蛇中的"吃"字标签
	GameLabel *m_title_third_word_label;	// 贪吃蛇中的"蛇"字标签
	RECT m_rect_scene;						// 场景的矩形
};

#endif // !__MAIN_MENU_SCENE_H__

