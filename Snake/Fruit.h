//++++++++++++++++++++++++++++++++++
// ʳ����
//----------------------------------

#pragma once

#ifndef __FRUIT_H__
#define __FRUIT_H__

class Fruit
{
public:
	Fruit(int score) : m_score(score), m_x(0), m_y(0), m_w(0), m_h(0) { }
	Fruit(int score, int x, int y, int w, int h);
	~Fruit();

public:
	// ʳ�����Ⱦ
	void Render(HDC hdc);					
	// ��������ʳ���λ�úͿ��
	void Reset(int score, int x, int y, int w, int h);	
	// ����ʳ���x����λ��
	int GetX();								
	// ����ʳ���y����λ��
	int GetY();								
	// ��ȡʳ��ķ���
	int GetScore();							

private:
	int m_score;	// ʳ��ķ���
	int m_x;		// ʳ���x����λ��
	int m_y;		// ʳ���y����λ��
	int m_w;		// ʳ��Ŀ��
	int m_h;		// ʳ��ĸ߶�
};

#endif // !__FRUIT_H__
