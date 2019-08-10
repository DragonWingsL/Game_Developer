//++++++++++++++++++++++++++++++++++
// 游戏场景抽象类
//----------------------------------

#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	// 初始化场景
	virtual void Init(HWND hWnd) = 0;
	// 渲染场景
	virtual void Render(HDC hdc) = 0;
	// 更新场景
	virtual void Update(DWORD dwDeltaTime) = 0;
	// 场景按键处理
	virtual void OnKeyDown(Keys key) = 0;
	// 窗口消息的处理
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

#endif // !__SCENE_H__

