
#include <windows.h>		// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <vector>			// vector��������ͷ�ļ�

#include "GameType.h"		// ��Ϸ��ʹ�õ���һЩö�١���������
#include "SnakeNode.h"		// ����ڵ���
#include "Snake.h"			// ̰������

Snake::Snake(int nMaxLenght) 
	: m_max_length(nMaxLenght),		// �趨�������󳤶�
	m_change_speed_percent(.01f),	// �趨�ٶȸı�İٷֱ�
	m_change_speed_count(6)		// �����ٶȸı���ݵ���
{
}

Snake::~Snake()
{
}

void Snake::Init(int latticeWidth, int latticeHeight, int xCenter, int yCenter)
{
	// ����ߵĽڵ�
	m_components.clear();

	// ��ʼ���ڵ��Ⱥ͸߶�
	m_component_width = latticeWidth;
	m_component_height = latticeHeight;
	// ���ú��������ÿ���ƶ��ľ���
	m_h_move_distance = m_component_width;
	m_v_move_distance = m_component_height;
	// �����ƶ��ӳ�
	m_move_delay = 130L;
	// ��ʼ�������ƶ���ʱ���
	m_update_delay = m_move_delay;
	// �趨�ɸı���ͷ����
	m_can_change_dir = true;

	/* *
	 * ��ʼ���ߵ����, �����������
	 * �ֱ���: ��ͷ��������β
	 * */

	int pos_arr[][2] = {
		{ xCenter, yCenter },
		{ xCenter, yCenter + m_component_height },
		{ xCenter, yCenter + 2 * m_component_height }
	};

	// ��ȡ��ͷ������(x, y) ��������ͷ 
	int x_head, y_head;
	x_head = pos_arr[0][0];
	y_head = pos_arr[0][1];
	SnakeNode head(x_head, y_head, m_component_width, m_component_height, SnakeNodeType::SNT_HEAD);
	head.SetDir(SnakeNodeDir::SND_UP);
	m_components.push_back(head);

	// �������� 
	// - 2 ����Ϊ����β�͵�������д���
	const int c_body = ARRAYSIZE(pos_arr) - 2;
	int i_body = 0;
	int x_body, y_body;
	while (true)
	{
		if (i_body >= c_body)
			break;

		x_body = pos_arr[i_body + 1][0];
		y_body = pos_arr[i_body + 1][1];
		SnakeNode body(x_body, y_body, m_component_width, m_component_height, SnakeNodeType::SNT_BODY);
		body.SetDir(SnakeNodeDir::SND_UP);
		m_components.push_back(body);
		++i_body;
	}

	// ������β 
	int x_tail, y_tail;
	x_tail = pos_arr[i_body + 1][0];
	y_tail = pos_arr[i_body + 1][1];
	SnakeNode tail(x_tail, y_tail, m_component_width, m_component_height, SnakeNodeType::SNT_TAIL);
	tail.SetDir(SnakeNodeDir::SND_UP);
	m_components.push_back(tail);
}

void Snake::Render(HDC hdc)
{
	// �����ߵ����,ͨ��������
	std::vector<SnakeNode>::iterator iter_component;
	iter_component = m_components.begin();
	// �����ڵ�
	while (true)
	{
		// �����������һ���ڵ�
		if (iter_component == m_components.end())
			break;
		// ��Ⱦ�ڵ�
		iter_component->Render(hdc);
		++iter_component;
	}
}

void Snake::Update(DWORD dwDeltaTime)
{
	// �ж��Ƿ���ͣ
	if (!m_is_pause)
	{
		// �ƶ���
		Move(dwDeltaTime);
	}
}

void Snake::OnKeyDown(Keys key)
{
	// ������һ���ƶ�ֻ�ܸı�һ����ͷ����
	if (m_can_change_dir)
	{
		// ���ݰ����ı���ͷ����
		SnakeNode &head = *m_components.begin();
		if ((key == Keys::KEY_W || key == Keys::KEY_UP) && head.GetDir() != SnakeNodeDir::SND_DOWN)
			head.SetDir(SnakeNodeDir::SND_UP);
		else if ((key == Keys::KEY_S || key == Keys::KEY_DOWN) && head.GetDir() != SnakeNodeDir::SND_UP)
			head.SetDir(SnakeNodeDir::SND_DOWN);
		else if ((key == Keys::KEY_A || key == Keys::KEY_LEFT) && head.GetDir() != SnakeNodeDir::SND_RIGHT)
			head.SetDir(SnakeNodeDir::SND_LEFT);
		else if ((key == Keys::KEY_D || key == Keys::KEY_RIGHT) && head.GetDir() != SnakeNodeDir::SND_LEFT)
			head.SetDir(SnakeNodeDir::SND_RIGHT);
		else
			return;

		// ���ø��·������
		m_can_change_dir = false;
	}
}

int Snake::GetNodeCount()
{
	// ��ȡ�ڵ���
	return m_components.size();
}

bool Snake::GetNodesPosition(int *&pPositionArray)
{
	if (NULL == pPositionArray)
		return false;

	// �����ڵ㣬���ڵ��x y����������
	size_t i;
	for (i = 0; i < m_components.size() * 2; i += 2)
	{
		pPositionArray[i] = m_components[i/2].GetX();
		pPositionArray[i + 1] = m_components[i/2].GetY();
	}
	return (i == m_components.size() * 2);
}

int Snake::GetHeadX()
{
	// ��ȡ��ͷx����
	return m_components.begin()->GetX();
}

int Snake::GetHeadY()
{
	// ��ȡ��ͷy����
	return m_components.begin()->GetY();
}

SnakeNodeDir Snake::GetHeadDir()
{
	// ��ȡ��ͷ����
	return m_components.begin()->GetDir();
}

void Snake::Move(DWORD dwDeltaTime)
{
	m_update_delay -= dwDeltaTime;

	// �����һ���ƶ�����ʱ������Ƚϣ�����ﵽ�ƶ����ʱ�䣬���ƶ�
	if (m_update_delay > 0)
		return;

	// �趨�ɸı���ͷ����
	m_can_change_dir = true;

	// �ƶ���,ͨ�����������
	std::vector<SnakeNode>::reverse_iterator rev_iter_component;
	rev_iter_component = m_components.rbegin();
	int x_body, y_body;
	while (true)
	{
		// ����������һ��,���˳�
		if (rev_iter_component == m_components.rend())
			break;
		// ���Ϊ�����ڶ���,��Ϊ��ͷ
		if ((rev_iter_component + 1) == m_components.rend())
		{
			// �ƶ���ͷ
			MoveHead(*rev_iter_component);
			break;
		}

		// �ƶ��ߵ������β��,ͨ���ƶ���ǰһ���ڵ��λ�ü�������ƶ�
		x_body = (rev_iter_component + 1)->GetX();
		y_body = (rev_iter_component + 1)->GetY();
		rev_iter_component->MoveTo(x_body, y_body);
		rev_iter_component->SetDir((rev_iter_component + 1)->GetDir());
		++rev_iter_component;
	}

	// ���浱ǰ���µ�ʱ���
	m_update_delay = m_move_delay;
}

void Snake::MoveHead(SnakeNode &head)
{
	// �ƶ���ͷ
	int x_head, y_head;
	GetSnakeHeadNextMovePosition(&x_head, &y_head, 0);
	head.MoveTo(x_head, y_head);
}

bool Snake::IsMaxLength()
{
	// ̰���ߵ������Ƿ�ﵽ��󳤶�
	return m_components.size() >= m_max_length;
}

void Snake::Pause()
{
	// ��̰ͣ���ߵ��ƶ�
	m_is_pause = true;
}

void Snake::Continue()
{
	// ����̰���ߵ��ƶ�
	m_is_pause = false;
}

bool Snake::IsEatSelf(DWORD dwDeltaTime)
{
	// �ж��Ƿ�ҧ������
	int x_head, y_head;
	// ��ȡ��ǰ��ͷ�ƶ�����λ��(ģ���ƶ�)
	GetSnakeHeadNextMovePosition(&x_head, &y_head, dwDeltaTime);
	// ���δ�ﵽ�ƶ�ʱ��, ��ͷ��δ�����ƶ�, ����Ҫ������, ֱ�ӷ���
	if (x_head == GetHeadX() && y_head == GetHeadY())
		return false;

	// �����ߵĽڵ�(����ͷ��ʼ,��Ϊ�ƶ�����ͷ�͸ı���,�ڶ����ڵ���ƶ�����ǰ��ͷ��λ��,��������ͷλ�ò�δ�ı�)
	for (size_t i = 0; i < m_components.size() - 1; ++i)
	{
		// ͨ���ж������ж���ͷ������Ľڵ��Ƿ��ص�,���ص�,��ҧ����
		if (m_components[i].GetX() == x_head && m_components[i].GetY() == y_head)
			return true;
	}
	return false;
}

void Snake::GetSnakeHeadNextMovePosition(int *pX, int *pY, DWORD dwDeltaTime)
{
	// ��ȡ��ͷ
	SnakeNode head = *m_components.begin();
	// ͨ�����㵱ǰʱ��������һ���ƶ���ʱ����Ƚ�, �Ƿ�ﵽ�ƶ�ʱ����
	if (m_update_delay - (LONG)dwDeltaTime > 0)
	{
		// δ����,���ص�ǰ��ͷλ��
		*pX = head.GetX();
		*pY = head.GetY();
		return;
	}
	// ����,�򷵻��ƶ����λ��(����ͷ��δ�ƶ�)

	// ǰ���ľ���(x, y)
	int x_forward = 0;
	int y_forward = 0;

	// x
	if (head.GetDir() == SnakeNodeDir::SND_LEFT)
		x_forward = -m_h_move_distance;
	else if (head.GetDir() == SnakeNodeDir::SND_RIGHT)
		x_forward = m_h_move_distance;

	// y
	if (head.GetDir() == SnakeNodeDir::SND_UP)
		y_forward = -m_v_move_distance;
	else if (head.GetDir() == SnakeNodeDir::SND_DOWN)
		y_forward = m_v_move_distance;

	*pX = head.GetX() + x_forward;
	*pY = head.GetY() + y_forward;
}

void Snake::Increase()
{
	// ����Ƿ�ﵽ�������󳤶�
	if (IsMaxLength())
		return;

	// ��ȡ���������ڵ�(��β�ڵ�)
	std::vector<SnakeNode>::iterator iter_insert = m_components.begin() + (m_components.size() - 1);
	// ��������ڵ�
	SnakeNode body(iter_insert->GetX(), iter_insert->GetY(), m_component_width, m_component_height, SnakeNodeType::SNT_BODY);
	body.SetDir(iter_insert->GetDir());
	// ���µ�����ڵ���뵽β�ͽڵ��ǰ��
	m_components.insert(iter_insert, body);

	// �������ĳ����Ƿ�ﵽ�ı��ٶȵ�����
	if (GetNodeCount() % m_change_speed_count == 0)
	{
		m_move_delay -= (DWORD)(m_move_delay * m_change_speed_percent);
	}
}
