
#include <windows.h>			// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <tchar.h>				// ͨ���ַ���ͷ�ļ�,�����ַ���֮��ļ��ݱ���ASCII�����Unicode����
#include <time.h>				// ʹ����ʱ����صĺ���(���������time())
#include <vector>				// vector��������ͷ�ļ�
#include <algorithm>			// �ṩ�˴������ڵ������ķǳ�Աģ�庯��

#include "GameType.h"			// ��Ϸ��ʹ�õ���һЩö�١���������
#include "Snake.h"				// ̰������
#include "Trap.h"				// �ϱ���
#include "Fruit.h"				// ʳ����
#include "util.h"				// ���ù�����
#include "GameBattleArea.h"		// ��Ϸ��ս����

GameBattleArea::GameBattleArea(RECT rcBattle, const int nHLatticeCount, const int nVLatticeCount) 
	: m_battle_rect(rcBattle),				// ��ʼ����ս�ľ���
	m_h_lattice_count(nHLatticeCount),		// ��ʼ����ս�ĺ��������
	m_v_lattice_count(nVLatticeCount)		// ��ʼ����ս�����������
{
	// ����̰����
	m_snake = new Snake(m_v_lattice_count * m_h_lattice_count);
	// ����ʳ��
	m_fruit = new Fruit(1);
	// ��ʼ�����ӿ�Ⱥ͸߶�
	m_lattice_width = (m_battle_rect.right - m_battle_rect.left) / m_h_lattice_count;
	m_lattice_height = (m_battle_rect.bottom - m_battle_rect.top) / m_v_lattice_count;
	// ��ʼ��ʧ�ܵ� "��" ��������
	m_defeat_tips_scale_rate = .3f;
	// ��ʼ���Ŵ�����
	m_is_scale_up = true;
}

GameBattleArea::~GameBattleArea()
{
	// �ͷ���Ϸ��Ϸ��ս������Դ
	SafeDeleteObject(m_snake);
	SafeDeleteObject(m_fruit);

	// ����ϱ�
	m_traps.clear();
}

void GameBattleArea::Init(HWND hWnd)
{
	// ����ϱ�
	m_traps.clear();

	// ��ʼ���ߵ�����λ��
	int center_x, center_y;
	center_x = m_battle_rect.left + (int)(m_h_lattice_count / 2 * m_lattice_width + m_lattice_width * .5f);
	center_y = m_battle_rect.top + (int)(m_v_lattice_count / 2 * m_lattice_height + m_lattice_height * .5f);

	// ��ʼ����
	m_snake->Init(m_lattice_width, m_lattice_height, center_x, center_y);

	// ����ˮ��
	RespawnFruit();

	// �����ϱ�
	SpawnTraps();
}

void GameBattleArea::Render(HDC hdc)
{
	//��Ⱦ��Ϸ��Ϸ��ս��

	HBRUSH hBattleBackgroundBrush = ::CreateSolidBrush(RGB(144, 81, 73));
	// �����Ϸ��ս���ı���
	::FillRect(hdc, &m_battle_rect, hBattleBackgroundBrush);

	// ��Ⱦ��
	m_snake->Render(hdc);

	if (m_fruit != NULL) {
		// ��Ⱦʳ��
		m_fruit->Render(hdc);
	}

	// ������Ⱦ�ϱ�
	for (int i = 0, count = m_traps.size(); i < count; ++i)
		m_traps[i].Render(hdc);

	// ��Ⱦʧ����ʾ�� "��"
	if (m_is_game_over)
		RenderDefeatTips(hdc);

	SafeDeleteGDIObject(hBattleBackgroundBrush);
}

void GameBattleArea::Update(DWORD dwDeltaTime)
{
	// ��Ϸ��ս���ĸ���

	// �ж��Ƿ���ͣ����Ϸ����
	if (!m_is_pause && !m_is_game_over)
	{
		// ��ͷ��ײ������
		SnakeHeadCollisionType collisionType = SnakeHeadCollisionType::SHCT_AIR;
		// ��ȡ��ͷ��ײ������
		GetSnakeHeadCollision(collisionType, dwDeltaTime);

		switch (collisionType)
		{
		case SHCT_SELF: // ҧ���Լ�
			m_is_game_over = true;
			m_game_over_type = GameOverType::GOT_SELF;
			fnGameOver();
			break;
		case SHCT_TRAP: // �����ϱ�
			m_is_game_over = true;
			m_game_over_type = GameOverType::GOT_TRAP;
			fnGameOver();
			break;
		case SHCT_FRUIT: // �Ե�ʳ��
			// ��������
			m_snake->Increase();
			// ����̰����
			m_snake->Update(dwDeltaTime);
			// �ж�̰�����Ƿ�ﵽ��󳤶�
			if (!m_snake->IsMaxLength())
			{
				// ���ӷ���
				fnBattleScoreAdd(m_fruit->GetScore());
				// ����ʳ��
				RespawnFruit();
			}
			else 
			{
				// ̰����Ӯ���˱���
				m_is_game_over = true;
				m_game_over_type = GameOverType::GOT_WIN;
				fnGameOver();
			}
			break;
		default:
			// ����̰����
			m_snake->Update(dwDeltaTime);
			break;
		}
	}

	// �ж��Ƿ���Ϸ����,��������ʧ�ܵ� "��"
	if (m_is_game_over)
	{
		// �ж��Ƿ�Ŵ�����
		if (m_is_scale_up)
		{
			// ����һ����������
			m_defeat_tips_scale_rate += dwDeltaTime * .001f;
			// �ﵽ�������,������С
			if (m_defeat_tips_scale_rate >= 1.0f)
			{
				m_defeat_tips_scale_rate = 1.0f;
				m_is_scale_up = false;
			}
		}
		else 
		{
			// ����һ����������
			m_defeat_tips_scale_rate -= dwDeltaTime * .001f;
			// �ﵽ��С����,���зŴ�
			if (m_defeat_tips_scale_rate <= .3f)
			{
				m_defeat_tips_scale_rate = .3f;
				m_is_scale_up = true;
			}
		}
	}
}

void GameBattleArea::OnKeyDown(Keys key)
{
	// �ж��Ƿ���ͣ �� ��Ϸ����
	if (m_is_pause || m_is_game_over)
		return;

	// ̰���߰�������
	m_snake->OnKeyDown(key);
}

void GameBattleArea::RespawnFruit()
{
	// ��ȡ����ڵ�����������ȷ���������������
	int array_count = m_snake->GetNodeCount();
	// ���������������д�������
	int *pNodesPositionArray = new int[array_count * 2];
	// ����������нڵ��λ�ñ��浽����
	// ����ĸ�ʽΪ: array[0]Ϊx����, array[1]Ϊy����
	m_snake->GetNodesPosition(pNodesPositionArray);

	// ��λ������������¼���Ϊ��������
	std::vector<int> nodes;
	int temp1 = 0, temp2 = 0;
	for (int i = 0, count = array_count * 2; i < count; i += 2)
	{
		// ���ߵĽڵ�x����λ��ת��Ϊ���ӵ�x����
		temp1 = (pNodesPositionArray[i] - m_battle_rect.left) / m_lattice_width - 1;
		// ���ߵĽڵ�y����λ��ת��Ϊ���ӵ�y����
		temp2 = (pNodesPositionArray[i + 1] - m_battle_rect.top) / m_lattice_height - 1;
		// ����������(x, y)����ֵ����
		// ��ֵ��ʽ: x + y * (��������� - 2(���2Ϊ�����ϱ�)),
		// ��������Ϊ�˷���ʹ����������ɾ���ǿո���
		// ��Ϊempties�����˳������������ֵ��ص�
		nodes.push_back(temp1 + temp2 * (m_h_lattice_count-2));
	}

	// �������ϱڵĸ��ӵ����궼���浽vector������
	std::vector<POINT> empties;
	for (int row = 1, row_count = m_v_lattice_count - 1; row < row_count; ++row)
	{
		for (int col = 1, col_count = m_h_lattice_count - 1; col < col_count; ++col)
		{
			// ������������POINT�ṹ����б���
			POINT pt; pt.x = col; pt.y = row;
			// ��ӵ�empties��
			empties.push_back(pt);
		}
	}

	temp1 = temp2 = 0;
	// ���ߵĽڵ�������ֵ�Խ���ķ�ʽ��������
	std::sort(nodes.begin(), nodes.end());
	// ͨ�����������ʹ����������ɾ���ǿսڵ�
	for (int i = 0, count = nodes.size(); i < count; ++i)
	{
		// temp1Ϊempties�зǿո��ӵ�����
		// items[i]Ϊ�ߵĽڵ����ڵĽڵ�����(����ֵ�ĸ�ʽ)
		// temp2Ϊ����empties��ɾ���ĸ�����
		temp1 = nodes[i] - temp2;
		// ʹ��erase����ɾ��ָ���ķǿո���
		empties.erase(empties.begin() + temp1);
		// �ۼ�ɾ���ĸ�����
		++temp2;
	}

	// ������ڿո���
	if (empties.size() > 0)
	{
		// �������
		srand((unsigned)time(NULL));
		// �����
		int rnd = rand() % empties.size();
		// ʳ��ĺ�������
		int x_fruit = empties[rnd].x;
		int y_fruit = empties[rnd].y;
		// ����ʳ���λ������
		x_fruit = m_battle_rect.left + x_fruit * m_lattice_width + m_lattice_width / 2;
		y_fruit = m_battle_rect.top + y_fruit * m_lattice_height + m_lattice_height / 2;
		// ���������µ�ʳ���λ������
		int score = rand() % 3;
		m_fruit->Reset(score == 0 ? 1 : score, x_fruit, y_fruit, m_lattice_width, m_lattice_height);
	}

	// �ͷ�ռ�õ���Դ
	empties.clear();
	nodes.clear();
	SafeDeleteArrayObject(pNodesPositionArray);
}

void GameBattleArea::GetSnakeHeadCollision(SnakeHeadCollisionType &collisionType, DWORD dwDeltaTime)
{
	// ��ͷ��ײ����
	collisionType = SnakeHeadCollisionType::SHCT_AIR;
	// ��������ƫ����
	const int x_offset = m_battle_rect.left + (int)(m_lattice_width * .5f);
	const int y_offset = m_battle_rect.top + (int)(m_lattice_height * .5f);
	// ��ͷλ������
	int x_snake_head, y_snake_head;
	m_snake->GetSnakeHeadNextMovePosition(&x_snake_head, &y_snake_head, dwDeltaTime);
	// �ؼ�����ͷλ��Ϊ��������
	int x_head_lattice = (x_snake_head - x_offset) / m_lattice_width;
	int y_head_lattice = (y_snake_head - y_offset) / m_lattice_height;

	// ��ͷλ�����ϱڵ�������
	if (x_head_lattice == 0 || x_head_lattice == m_h_lattice_count-1 ||
		y_head_lattice == 0 || y_head_lattice == m_v_lattice_count-1)
	{
		collisionType = SnakeHeadCollisionType::SHCT_TRAP;
		return;
	}

	int x_fruit_lattice = (m_fruit->GetX() - x_offset) / m_lattice_width;
	int y_fruit_lattice = (m_fruit->GetY() - y_offset) / m_lattice_height;
	// ��ͷλ����ʳ���λ����
	if (x_head_lattice == x_fruit_lattice && y_head_lattice == y_fruit_lattice)
	{
		collisionType = SnakeHeadCollisionType::SHCT_FRUIT;
		return;
	}

	// ��ͷ��λ����������
	if (m_snake->IsEatSelf(dwDeltaTime))
	{
		collisionType = SnakeHeadCollisionType::SHCT_SELF;
		return;
	}
}

void GameBattleArea::Pause()
{
	// ��ͣ��ս
	m_is_pause = true;
	// ��̰ͣ���ߵ��ƶ�
	m_snake->Pause();
}

void GameBattleArea::Continue()
{
	// ������ս
	m_is_pause = false;
	// ����̰���ߵ��ƶ�
	m_snake->Continue();
}

void GameBattleArea::SpawnTraps()
{
	// ��ʼ��Ĭ���ϱڸ�ʽ
	EnumTrapStyle trapStyle = EnumTrapStyle::TS_DOWNTIP;
	// ��������
	for (int row = 0; row < m_v_lattice_count; ++row)
	{
		for (int col = 0; col < m_h_lattice_count; ++col)
		{
			// �������Ϊ��һ�� ���� ���һ��
			if (row == 0 || row == m_v_lattice_count - 1)
			{
				// ���Ϊ��һ�� ���� ���һ�� (��Ϊ�ĸ���)
				// ��һ��Ϊx   ��һ��Ϊy   ��(0, 0)
				// ���һ��Ϊx ��һ��Ϊy   ��(h, 0)
				// ��һ��Ϊx   ���һ��Ϊy ��(0, v)
				// ��һ��Ϊx   ���һ��Ϊy ��(h, v)
				if (col == 0 || col == m_h_lattice_count - 1)
				{
					// �����ϱ���ʽΪ��
					trapStyle = EnumTrapStyle::TS_BLOCK;
				}
				else
				{
					// �м䲿��, ��������
					// ����Ϊ��һ��,��Ϊ���µĴ�
					// ����Ϊ���һ��,��Ϊ���ϵĴ�
					trapStyle = row == 0 ? EnumTrapStyle::TS_UPTIP : EnumTrapStyle::TS_DOWNTIP;
				}
			}
			// ���Ϊ��һ��
			else if (col == 0)
			{
				// ��Ϊ��ߵĴ�
				trapStyle = EnumTrapStyle::TS_LEFTTIP;
			}
			// ���Ϊ���һ��
			else if (col == m_h_lattice_count - 1)
			{
				// ��Ϊ�ұߵĴ�
				trapStyle = EnumTrapStyle::TS_RIGHTTIP;
			}
			else
				// ��������ıߵĸ���,������
				continue;

			// �����ϱ�
			Trap trap(trapStyle, (int)(m_battle_rect.left + col * m_lattice_width + m_lattice_width * .5f),
				(int)(m_battle_rect.top + row * m_lattice_height + m_lattice_height * .5f), m_lattice_width, m_lattice_height);
			// ��ӵ������ϱڵ�������
			m_traps.push_back(trap);
		}
	}
}

bool GameBattleArea::IsGameOver()
{
	// �����Ƿ��ս����
	return m_is_game_over;
}

bool GameBattleArea::IsPause()
{
	return m_is_pause;
}

void GameBattleArea::Restart(HWND hWnd)
{
	// ���¿�ʼ��ս
	m_is_game_over = false;
	m_is_pause = false;
	// ���³�ʼ����ս
	Init(hWnd);
}

GameOverType GameBattleArea::GetGameOverType()
{
	// ��Ϸ����������
	return m_game_over_type;
}

void GameBattleArea::RenderDefeatTips(HDC hdc)
{
	// �ж��Ƿ�ʤ��,�򷵻�
	if (m_game_over_type == GameOverType::GOT_WIN)
		return;

	HPEN hPen = ::CreatePen(PS_SOLID, 6, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	// ��ȡ��ͷλ������
	int x_center = m_snake->GetHeadX();
	int y_center = m_snake->GetHeadY();

	// ������ͷλ��,���м��϶�Ӧ��ƫ����,��Ϊ��ײ����λ��
#if false
	// �պ��ſ���...o((�ѩn��))o
	"
	 * 
	 *       \   / <----- ʧ�ܵ���ʾ
	 *       *****  
	 *      **/ \**   
	 *     **     **  
	 *    **       ** 
	 *   *************
	 *   *************
	 *   *************
	 *
	 "
#endif 
	switch (m_snake->GetHeadDir())
	{
	case SND_UP:
		y_center -= (int)(m_lattice_height * .5f);
		break;
	case SND_DOWN:
		y_center += (int)(m_lattice_height * .5f);
		break;
	case SND_LEFT:
		x_center -= (int)(m_lattice_width * .5f);
		break;
	case SND_RIGHT:
		x_center += (int)(m_lattice_width * .5f);
		break;
	default:
		break;
	}

	/* ���� "��" */
	float len = m_lattice_width * .5f * m_defeat_tips_scale_rate;
	float angles[] = {
		45, 135, 225, 315
	};

	int points[2][4];

	float rad = 3.1415926f / 180.0f;
	/* ��һ�� */
	int x_lt = (int)(x_center + len * cos(angles[0] * rad));
	int y_lt = (int)(y_center + len * sin(angles[0] * rad));
	int x_rd = (int)(x_center + len * cos(angles[2] * rad));
	int y_rd = (int)(y_center + len * sin(angles[2] * rad));

	/* �ڶ��� */
	int x_rt = (int)(x_center + len * cos(angles[1] * rad));
	int y_rt = (int)(y_center + len * sin(angles[1] * rad));
	int x_ld = (int)(x_center + len * cos(angles[3] * rad));
	int y_ld = (int)(y_center + len * sin(angles[3] * rad));

	points[0][0] = x_lt;
	points[0][1] = y_lt;
	points[0][2] = x_rd;
	points[0][3] = y_rd;

	points[1][0] = x_rt;
	points[1][1] = y_rt;
	points[1][2] = x_ld;
	points[1][3] = y_ld;

	/* ���� */
	Util::DrawLine(hdc, points[0]);
	Util::DrawLine(hdc, points[1]);

	::SelectObject(hdc, hOldPen);
	SafeDeleteGDIObject(hPen);
}

