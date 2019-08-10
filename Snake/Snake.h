//++++++++++++++++++++++++++++++++++
// ̰������
//----------------------------------

#pragma once

#ifndef __SNAKE_H__
#define __SNAKE_H__

class SnakeNode;			// ����̰���ߵĽڵ���
enum Keys;					// ����������ö��

class Snake
{
public:
	Snake(int nMaxLenght);
	~Snake();

public:
	// ��ʼ��̰����
	void Init(int latticeWidth, int latticeHeight, int xCenter, int yCenter);	
	// ��Ⱦ̰����
	void Render(HDC hdc);									
	// ����̰����
	void Update(DWORD dwDeltaTime);							
	// ��������
	void OnKeyDown(Keys key);								

	// �ƶ�̰����
	void Move(DWORD dwDeltaTime);							
	// ̰��������
	void Increase();										
	// ��ͣ�ƶ�̰����
	void Pause();											
	// �����ƶ�̰����
	void Continue();										
	// ��ȡ̰���ߵ���ͷ�´��ƶ�����λ��
	void GetSnakeHeadNextMovePosition(int *pX, int *pY, DWORD dwDeltaTime);	
	// ��ȡ̰���ߵĽڵ���
	int GetNodeCount();										
	// ��ȡ��ͷ��x����λ��
	int GetHeadX();											
	// ��ȡ��ͷ��y����λ��
	int GetHeadY();											
	// ��ȡ��ͷ����
	SnakeNodeDir GetHeadDir();								
	// ��ȡ̰���ߵ�λ��
	bool GetNodesPosition(int *&pPositionArray);			
	// �Ƿ�ҧ������
	bool IsEatSelf(DWORD dwDeltaTime);										
	// ̰���ߵ������Ƿ�ﵽ��󳤶�
	bool IsMaxLength();										

private:
	// �ƶ�̰���ߵ���ͷ
	void MoveHead(SnakeNode &head);							

private:
	std::vector<SnakeNode> m_components;	// �ߵĽڵ�
	DWORD m_move_delay;						// ��ÿ���ƶ���ʱ���(������)
	LONG m_update_delay;					// �����һ�θ��µ�ʱ���
	float m_change_speed_percent;			// �޸��ƶ��ٶȵı���
	int m_change_speed_count;				// �趨����ﵽһ���̶Ⱥ��޸��ƶ��ٶ�
	int m_h_move_distance;					// ��ͷÿ���ƶ��ĺ������
	int m_v_move_distance;					// ��ͷÿ���ƶ����������
	int m_component_width;					// �ߵĽڵ���
	int m_component_height;					// �ߵĽڵ�߶�
	size_t m_max_length;						// �������󳤶�
	bool m_is_pause;						// �Ƿ���ͣ�ߵ��ƶ�
	bool m_can_change_dir;					// �˴ΰ����Ƿ���Ըı���ͷ�ķ���
};

#endif // !__SNAKE_H__
