
#include <windows.h>		// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <cmath>			// ��ѧ��ͷ�ļ�,��������ೣ�õ���ѧ����

#include "GameType.h"		// ��Ϸ��ʹ�õ���һЩö�١���������
#include "util.h"			// ���ù�����
#include "SnakeNode.h"		// �ߵĽڵ���

SnakeNode::SnakeNode(int x, int y, int width, int height, SnakeNodeType type) 
	: m_x(x),		// ��ʼ���ߵĽڵ�x����λ��
	m_y(y),			// ��ʼ���ߵĽڵ�y����λ��
	m_w(width),		// ��ʼ���ߵĽڵ���
	m_h(height),	// ��ʼ���ߵĽڵ�߶�
	m_type(type)	// ��ʼ���ߵĽڵ�����
{
}


SnakeNode::~SnakeNode()
{
}

void SnakeNode::Render(HDC hdc)
{
	/* *
	 * ���ݲ�ͬ��λ���в�ͬ����Ⱦ
	 * */
	
	if (m_type == SnakeNodeType::SNT_HEAD)
	{
		// ��Ⱦ��ͷ�ڵ�
		RenderHead(hdc);
	}
	else if (m_type == SnakeNodeType::SNT_TAIL)
	{
		// ��Ⱦ��β�ڵ�
		RenderTail(hdc);
	}
	else
	{
		// ��Ⱦ����ڵ�
		RenderBody(hdc);
	}

#pragma region ��Ⱦ�߿�
#if RENDER_BORDER
	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	::Rectangle(hdc, m_x - m_w / 2, m_y - m_h / 2, m_x + m_w / 2, m_y + m_h / 2);

	::SelectObject(hdc, hOldPen);
	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hPen);
#endif 
#pragma endregion
}

void SnakeNode::MoveTo(int x, int y)
{
	// �ƶ������λ��
	m_x = x;
	m_y = y;
}

void SnakeNode::SetDir(SnakeNodeDir dir)
{
	// ���ýڵ㷽��
	m_dir = dir;
}

SnakeNodeDir SnakeNode::GetDir()
{
	// ���ؽڵ㷽��
	return m_dir;
}

int SnakeNode::GetX()
{
	// ����x����
	return m_x;
}

int SnakeNode::GetY()
{
	// ����y����
	return m_y;
}

void SnakeNode::RenderHead(HDC hdc)
{
	// ����ͷ��
	// ����r,���Ի��ư�Բ
	static const int r = (int)(m_w * .5f);
	// �ڵ�����ĵ�����λ��
	const int x_up_center = m_x;
	const int y_up_center = m_y;

	// �°벿�ֵľ���
	RECT rc_down;
	// �ϰ벿�ֵİ�Բ���Ƕ�
	float start_angle = .0f;
	// �����ĸ�������м���(��Ϊ�̶�����,�ñʺ�ֽ��һ�¾���֪����)
	if (m_dir == SnakeNodeDir::SND_UP)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y;
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y + (int)round(m_h * .5f);
	}
	else if (m_dir == SnakeNodeDir::SND_DOWN)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y;
		start_angle = 180.0f;
	}
	else if (m_dir == SnakeNodeDir::SND_LEFT)
	{
		rc_down.left = m_x;
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x + (int)round(m_w * .5f);
		rc_down.bottom = m_y + (int)round(m_h * .5f);
		start_angle = 90.0f;
	}
	else if (m_dir == SnakeNodeDir::SND_RIGHT)
	{
		rc_down.left = m_x - (int)round(m_w * .5f);
		rc_down.top = m_y - (int)round(m_h * .5f);
		rc_down.right = m_x;
		rc_down.bottom = m_y + (int)round(m_h * .5f);
		start_angle = 270.0f;
	}

	// ���л���
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 105, 180));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// �����ϰ벿�ֵ�·��
	::BeginPath(hdc);
	::MoveToEx(hdc, x_up_center, y_up_center, NULL);
	::AngleArc(hdc, x_up_center, y_up_center, r, start_angle, 180.0f);
	::EndPath(hdc);

	// ����ϰ벿��
	::FillPath(hdc);
	// �����°벿��
	::FillRect(hdc, &rc_down, hBrush);

	::SelectObject(hdc, hOldBrush);
	SafeDeleteGDIObject(hBrush);
}

void SnakeNode::RenderTail(HDC hdc)
{
	// β�ͻ���������
	const int x_center = m_x;
	const int y_center = m_y;
	static const int r = (int)round(Util::GetDistanceBetweenTwoPoint(x_center - m_w * .5f, y_center - m_h * .5f, x_center, y_center));
	static const int r2 = (int)round(m_w * .5f);

	/* *
	 * �����ε�������
	 * �ĸ�����,�ֱ�Ϊ�ĸ��Ƕ�
	 * */
	static const int angles[4][3] = {
		{45,  270, 135},/* up    */
		{315, 90,  225},/* down  */
		{45,  180, 315},/* left  */
		{135, 0,   225} /* rigth */
	};

	// ���ݵ�ǰ�Ƕ�ѡ�����涨��ĽǶ�����
	int angle_index;
	if (m_dir == SnakeNodeDir::SND_UP)
		angle_index = 0;
	else if (m_dir == SnakeNodeDir::SND_DOWN)
		angle_index = 1;
	else if (m_dir == SnakeNodeDir::SND_LEFT)
		angle_index = 2;
	else if (m_dir == SnakeNodeDir::SND_RIGHT)
		angle_index = 3;

	// ���ݵ�ǰβ�ͷ�����л��� β��"��"
	POINT points[3];
	points[0].x = x_center - (int)round((cos(angles[angle_index][0] * Util::RAD)) * r);
	points[0].y = y_center - (int)round((sin(angles[angle_index][0] * Util::RAD)) * r);

	points[1].x = x_center - (int)round((cos(angles[angle_index][1] * Util::RAD)) * r2);
	points[1].y = y_center - (int)round((sin(angles[angle_index][1] * Util::RAD)) * r2);

	points[2].x = x_center - (int)round((cos(angles[angle_index][2] * Util::RAD)) * r);
	points[2].y = y_center - (int)round((sin(angles[angle_index][2] * Util::RAD)) * r);

	// ����Util�Ķ�������
	Util::PolyFill(hdc, RGB(255, 192, 203), points, 3);
}

void SnakeNode::RenderBody(HDC hdc)
{
	// �������
	RECT rc_body;
	rc_body.left	= m_x - (int)round(m_w * .5f);
	rc_body.top		= m_y - (int)round(m_h * .5f);
	rc_body.right	= m_x + (int)round(m_w * .5f);
	rc_body.bottom	= m_y + (int)round(m_h * .5f);

	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 155, 190));
	// ��������
	::FillRect(hdc, &rc_body, hBrush);
	SafeDeleteGDIObject(hBrush);
}