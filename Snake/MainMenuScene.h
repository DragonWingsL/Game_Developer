//++++++++++++++++++++++++++++++++++
// ��Ϸ���˵�������
//----------------------------------

#pragma once

#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

class Game;			// ������Ϸ��
class GameButton;	// ������Ϸ��ť��
class GameLabel;	// ������Ϸ��ǩ��

// �̳�����Ϸ������
class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() override;

public:
	// ��ʼ������
	virtual void Init(HWND hWnd) override;						
	// ��Ⱦ����
	virtual void Render(HDC hdc) override;						
	// ���³���
	virtual void Update(DWORD dwDeltaTime) override;			
	// ������������
	virtual void OnKeyDown(Keys key) override;					
	// ������Ϣ�Ĵ���
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;	

	// ��Ϸ��ʼ��ť����¼�
	void OnStartGameButtonClicked(int x, int y);				
	// �˳���Ϸ��ť����¼�
	void OnExitGameButtonClicked(int x, int y);					

public:
	GameDelegateFunction fnExitGame;		// �˳���Ϸ�ĺ���(function)����
	GameDelegateFunction fnSwitchToGame;	// �л�����Ϸ�����ĺ���(function)����

private:
	GameButton *m_start_game_button;		// ��ʼ��Ϸ��ť
	GameButton *m_exit_game_button;			// �˳���Ϸ��ť
	GameLabel *m_title_first_word_label;	// ̰�����е�"̰"�ֱ�ǩ
	GameLabel *m_title_second_word_label;	// ̰�����е�"��"�ֱ�ǩ
	GameLabel *m_title_third_word_label;	// ̰�����е�"��"�ֱ�ǩ
	RECT m_rect_scene;						// �����ľ���
};

#endif // !__MAIN_MENU_SCENE_H__

