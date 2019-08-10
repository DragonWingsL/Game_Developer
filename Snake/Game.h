//++++++++++++++++++++++++++++++++++
// ��Ϸ��
//----------------------------------

#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// ����������
class Scene;
// ��������ö��
enum Keys;

// ö����Ϸ����
enum EnumGameScene {
	SCENE_MAINMENU, SCENE_GAME
};

class Game
{
public:
	Game();
	~Game();

public:
	// ��ʼ����Ϸ
	void Init(HWND hWnd);
	// ��Ⱦ��Ϸ
	void Render(HDC hdc);
	// ������Ϸ
	void Update(DWORD dwDeltaTime);
	// ��Ϸ��������
	void OnKeyDown(Keys key);
	// ������Ϣ�Ĵ���
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//�л������˵�����
	void SwitchSceneToMainMenu();
	// �л�����Ϸ����
	void SwitchSceneToGame();
	// �˳���Ϸ
	void ExitGame();
	// �����л������˵��ı�־(����һ�������߼��н����л�����)
	void SetSwitchToMainMenuFlag();
	// �����л�����Ϸ�����ı�־(����һ�������߼��н����л�����)
	void SetSwitchToGameFlag();

public:
	// �ṩ��̬�ķ����������ȡ��Ϸ�Ĵ��ھ��
	static const HWND GetGameHwnd() { return m_hWnd; }

private:
	static HWND m_hWnd;				// ��Ϸ���ھ��
	Scene *m_scene;					// ��ǰ����Ϸ����
	EnumGameScene m_scene_flag;		// ��Ϸ�����ı�־
	bool m_is_switch_scene;			// �Ƿ���Ҫ����һ�������߼����л�����
};

#endif // !__GAME_H__
