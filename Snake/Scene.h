//++++++++++++++++++++++++++++++++++
// ��Ϸ����������
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
	// ��ʼ������
	virtual void Init(HWND hWnd) = 0;
	// ��Ⱦ����
	virtual void Render(HDC hdc) = 0;
	// ���³���
	virtual void Update(DWORD dwDeltaTime) = 0;
	// ������������
	virtual void OnKeyDown(Keys key) = 0;
	// ������Ϣ�Ĵ���
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

#endif // !__SCENE_H__

