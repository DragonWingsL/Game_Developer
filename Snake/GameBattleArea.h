//++++++++++++++++++++++++++++++++++
// ��Ϸ��Ϸ��ս����
//----------------------------------

#pragma once

#ifndef __GAME_BATTLE_AREA_H__
#define __GAME_BATTLE_AREA_H__

class Snake;					// ����̰������
class Fruit;					// ����ʳ����
class Trap;						// �����ϱ���
enum SnakeHeadCollisionType;	// ������ͷ��ײö��

class GameBattleArea
{
public:
	GameBattleArea(RECT rcBattle, const int nHLatticeCount, const int nVLatticeCount);
	~GameBattleArea();

public:
	// ��ʼ������
	void Init(HWND hWnd);					
	// ��Ⱦ����
	void Render(HDC hdc);					
	// ���³���
	void Update(DWORD dwDeltaTime);			
	// ������������
	void OnKeyDown(Keys key);				

	// �����ͷ����ײ
	void GetSnakeHeadCollision(SnakeHeadCollisionType &collisionType, DWORD dwDeltaTime);	
	// ��ս��ͣ
	void Pause();							
	// ��ս����
	void Continue();						
	// ��Ϸ��Ϸ��ս�����¿�ʼ
	void Restart(HWND hWnd);				
	// ��ȡ��Ϸ����������
	GameOverType GetGameOverType();			
	// ��Ϸ��Ϸ��ս���Ƿ���Ϸ����
	bool IsGameOver();						
	// �Ƿ���ͣ��Ϸ
	bool IsPause();							

private:
	// ����ʳ��
	void RespawnFruit();					
	// �����ϱ�
	void SpawnTraps();						
	// ��Ⱦʧ����ʾ
	void RenderDefeatTips(HDC hdc);			

public:
	GameDelegateFunction fnGameOver;					// ��Ϸ�����ĺ���(function)����
	GameBattleScoreDelegateFunction fnBattleScoreAdd;	// ��ս�������ӵĺ���(function)����

private:
	std::vector<Trap> m_traps;		// �����е��ϱ�
	Snake *m_snake;					// ̰����
	Fruit *m_fruit;					// �����е�ʳ��
	RECT m_battle_rect;				// �������
	GameOverType m_game_over_type;	// ��Ϸ��������
	const int m_h_lattice_count;	// �����еĺ��������
	const int m_v_lattice_count;	// �����е����������
	int m_lattice_width;			// �����еĸ��ӿ��
	int m_lattice_height;			// �����еĸ��Ӹ߶�
	bool m_is_pause;				// �Ƿ���ͣ��ս
	bool m_is_game_over;			// �Ƿ��ս����
	bool m_is_scale_up;				// �Ŵ������дʧ����ʾ��"��"
	float m_defeat_tips_scale_rate; // ʧ����ʾ��"��"���ű���
};

#endif // !__GAME_BATTLE_AREA_H__