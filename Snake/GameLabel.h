//++++++++++++++++++++++++++++++++++
// ��Ϸ��ǩ��
//----------------------------------

#pragma once

#ifndef __GAME_LABEL_H__
#define __GAME_LABEL_H__

class GameLabel
{
public:
	GameLabel(int x = 0, int y = 0, LPCTSTR text = NULL);
	~GameLabel();

public:
	// ��Ⱦ��ǩ
	void Render(HDC hdc);								
	// ���ñ�ǩ�ı�
	void SetText(LPCTSTR text);							
	// ���ñ�ǩ����ߴ�
	void SetFontSize(int nHeight);						
	// ���ñ�ǩx����λ��
	void SetX(int x);									
	// ���ñ�ǩy����λ��
	void SetY(int y);									
	// ���ñ�ǩ������ɫ
	void SetBackground(COLORREF background);			
	// ���ñ�ǩǰ����ɫ
	void SetForeground(COLORREF foreground);			
	// ���ñ�ǩ�����Ƿ�͸��
	void SetBackgroundTransparent(bool bTransparent);	
	// ���ñ�ǩ�Ƿ�����
	void SetEnable(bool bEnable);						
	// ���ñ�ǩ�Ƿ�ɼ�
	void SetVisible(bool bVisible);						
	// ��ȡ��ǩ�ı�
	LPCTSTR GetText();									
	// ��ȡ��ǩx����λ��
	int GetX();											
	// ��ȡ��ǩy����λ��
	int GetY();											
	// ��ȡ��ǩ���
	int GetWidth();										
	// ��ȡ��ǩ���
	int GetHeight();									
	// �Ƿ�ɼ�
	bool IsVisible();									
	// �Ƿ�����
	bool IsEnable();									

protected:
	int m_x;						// ��ǩx����λ��
	int m_y;						// ��ǩy����λ��	
	int m_w;						// ��ǩ���
	int m_h;						// ��ǩ�߶�
	bool m_background_transparent;	// ��ǩ�����Ƿ�͸��
	bool m_enable;					// ��ǩ�Ƿ�����
	bool m_visible;					// ��ǩ�Ƿ�ɼ�
	COLORREF m_background;			// ��ǩ�ı�����ɫ
	COLORREF m_foreground;			// ��ǩ��ǰ����ɫ
	LPCTSTR m_text;					// ��ǩ���ı�
};

#endif // !__GAME_LABEL_H__
