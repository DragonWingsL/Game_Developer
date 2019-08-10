//++++++++++++++++++++++++++++++++++
// �ϱ���
//----------------------------------

#pragma once

#ifndef __TRAP_H__
#define __TRAP_H__

enum EnumTrapStyle;	// �����ϱ���ʽö��

class Trap 
{
public:
	Trap(EnumTrapStyle style, int x, int y, int w, int h);
	~Trap();

public:
	// ʳ�����Ⱦ
	void Render(HDC hdc);			

	// ����ʳ���x����λ��
	int GetX();						
	// ����ʳ���y����λ��
	int GetY();						

private:
	EnumTrapStyle m_style;	// �ϱڵ���ʽ
	int m_x;				// ʳ���X����λ��
	int m_y;				// ʳ���Y����λ��
	int m_w;				// ʳ��Ŀ��
	int m_h;				// ʳ��ĸ߶�
};

#endif // !__TRAP_H__
