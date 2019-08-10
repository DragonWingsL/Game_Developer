//++++++++++++++++++++++++++++++++++
// ��Ϸ������
//----------------------------------

#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

class GameButton;		// ������Ϸ��ť��
class GameBattleArea;	// ������Ϸ��Ϸ��ս����
class GameLabel;		// ������Ϸ��ǩ��

// �̳�����Ϸ������
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene() override;

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

	// ��Ϸ��ͣ��ť����¼�
	void OnPuaseGameButtonClicked(int x, int y);	
	// �˳���Ϸ��ť����¼�
	void OnExitGameButtonClicked(int x, int y);	
	// ���¿�ʼ��Ϸ��ť����¼�
	void OnNewGameButtonClicked(int x, int y);	
	// �л������˵���ť����¼�
	void OnSwitchMainMenuButtonClicked(int x, int y);	
	// ��Ϸ�����¼�
	void OnGameOver();				
	// ���������¼�
	void OnGameScoreAdd(int score);	

private:
	// ������Ϸ����
	void RenderGameOver(HDC hdc);	

public:
	GameDelegateFunction fnExitGame;			// �˳���Ϸ�ĺ���(function)����
	GameDelegateFunction fnSwitchToMainMenu;	// �л������˵��ĺ���(function)����

private:
	HWND m_hWnd;								// ��Ϸ���ھ��
	GameBattleArea *m_battle_area;				// ��Ϸ��Ϸ��ս��
	GameButton *m_pause_button;					// ��Ϸ��ͣ��ť
	GameButton *m_new_game_button;				// ���¿�ʼ��ť
	GameButton *m_switch_to_main_menu_button;	// �л������˵���ť
	GameButton *m_exit_button;					// �˳���Ϸ��ť
	GameLabel *m_score_tips_title;				// ��������
	GameLabel *m_score_tips;					// ����
	GameLabel *m_score_add_tips;				// ����������ʾ
	GameLabel *m_game_time_tips_title;			// ��Ϸʱ�����
	GameLabel *m_game_time_tips;				// ��Ϸʱ��
	GameLabel *m_game_over_title_tips;			// ��Ϸ��������
	GameLabel *m_game_over_reason_tips;			// ��Ϸ����ԭ��
	LPTSTR m_score_string;						// �������ַ���������
	LPTSTR m_score_add_string;					// ����������ʾ���ַ���������
	LPTSTR m_game_time_string;					// ��Ϸʱ����ַ���������
	DWORD m_game_tick;							// ��Ϸʱ��(������)
	RECT m_rect;								// �����������
	RECT m_game_over_panel_rect;				// ��Ϸ�������ľ���
	const LONG m_score_add_tips_visible_time;	// �������ӿɼ�ʱ��
	LONG m_score_add_tips_time;					// �������Ӿ������ص�ʱ��
	int m_scene_width;							// �������
	int m_scene_height;							// �����߶�
	int m_game_over_panel_width;				// ��Ϸ�������Ŀ��
	int m_game_over_panel_height;				// ��Ϸ�������ĸ߶�
	int m_game_over_x_origin;					// ��Ϸ�����������ĵ�x����
	int m_game_over_y_origin;					// ��Ϸ�����������ĵ�y���
	int m_score;								// ��Ϸ����
};

#endif // !__GAME_SCENE_H__
 