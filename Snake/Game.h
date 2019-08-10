//++++++++++++++++++++++++++++++++++
// 游戏类
//----------------------------------

#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// 声明场景类
class Scene;
// 声明按键枚举
enum Keys;

// 枚举游戏场景
enum EnumGameScene {
	SCENE_MAINMENU, SCENE_GAME
};

class Game
{
public:
	Game();
	~Game();

public:
	// 初始化游戏
	void Init(HWND hWnd);
	// 渲染游戏
	void Render(HDC hdc);
	// 更新游戏
	void Update(DWORD dwDeltaTime);
	// 游戏按键处理
	void OnKeyDown(Keys key);
	// 窗口消息的处理
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//切换到主菜单场景
	void SwitchSceneToMainMenu();
	// 切换到游戏场景
	void SwitchSceneToGame();
	// 退出游戏
	void ExitGame();
	// 设置切换到主菜单的标志(在下一个更新逻辑中进行切换场景)
	void SetSwitchToMainMenuFlag();
	// 设置切换到游戏场景的标志(在下一个更新逻辑中进行切换场景)
	void SetSwitchToGameFlag();

public:
	// 提供静态的方法，方便获取游戏的窗口句柄
	static const HWND GetGameHwnd() { return m_hWnd; }

private:
	static HWND m_hWnd;				// 游戏窗口句柄
	Scene *m_scene;					// 当前的游戏场景
	EnumGameScene m_scene_flag;		// 游戏场景的标志
	bool m_is_switch_scene;			// 是否需要在下一个更新逻辑中切换场景
};

#endif // !__GAME_H__
