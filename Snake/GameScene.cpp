
#include <windows.h>			// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)
#include <tchar.h>				// ͨ���ַ���ͷ�ļ�,�����ַ���֮��ļ��ݱ���ASCII�����Unicode����
#include <vector>				// vector��������ͷ�ļ�

#include "GameType.h"			// ��Ϸ��ʹ�õ���һЩö�١���������
#include "Scene.h"				// ����������
#include "GameButton.h"			// ��Ϸ��ť��
#include "GameLabel.h"			// ��Ϸ��ǩ��
#include "GameBattleArea.h"		// ��Ϸ��ս����
#include "Util.h"				// ���ù�����
#include "GameScene.h"			// ��Ϸ������

// Game.cpp ���Ѿ�������,�˴��Ͳ���˵��.
using std::bind;
using namespace std::placeholders; // C++11������-ռλ��, �����std::bindʹ��

GameScene::GameScene() 
	: m_score_add_tips_visible_time(500L),					// ���÷�������ʾ�Ŀɼ�ʱ��
	m_score_add_tips_time(m_score_add_tips_visible_time)	// ��ʼ������������ʾ��ǰ����ʾ��ʱ��
{
	// ������ť��.......(�����ֹ�)
	m_pause_button = new GameButton(0, 0, _T("��ͣ��Ϸ"));
	m_new_game_button = new GameButton(0, 0, _T("���¿�ʼ"));
	m_switch_to_main_menu_button = new GameButton(0, 0, _T("���˵�"));
	m_exit_button = new GameButton(0, 0, _T("�˳���Ϸ"));

	// ������ǩ��.......(�����ֹ�)
	m_game_over_title_tips = new GameLabel(0, 0, _T("��Ϸ����"));
	m_game_over_reason_tips = new GameLabel();
	m_score_tips = new GameLabel(0, 0, _T("0"));
	m_score_tips_title = new GameLabel(0, 0, _T("����: "));
	m_score_add_tips = new GameLabel();
	m_game_time_tips = new GameLabel(0, 0, _T("00:00:00"));
	m_game_time_tips_title = new GameLabel(0, 0, _T("��Ϸʱ��: "));

	// �������ӷ������ɼ�
	m_score_add_tips->SetVisible(false);

	// ������Ϸ������ʾ��Ԫ�ز��ɼ�
	m_game_over_title_tips->SetVisible(false);
	m_game_over_reason_tips->SetVisible(false);

	m_new_game_button->SetVisible(false);
	m_switch_to_main_menu_button->SetVisible(false);
	m_exit_button->SetVisible(false);

	// Ϊ��������Ϸʱ����ַ��������������ڴ�
	m_score_string = new TCHAR[32];
	m_game_time_string = new TCHAR[16];
	m_score_add_string = new TCHAR[8];
}

GameScene::~GameScene()
{
	// �ͷ���Ϸ��������Դ
	SafeDeleteObject(m_pause_button);
	SafeDeleteObject(m_new_game_button);
	SafeDeleteObject(m_switch_to_main_menu_button);
	SafeDeleteObject(m_exit_button);
	SafeDeleteObject(m_game_over_title_tips);
	SafeDeleteObject(m_game_over_reason_tips);
	SafeDeleteObject(m_score_tips);
	SafeDeleteObject(m_score_tips_title);
	SafeDeleteObject(m_game_time_tips);
	SafeDeleteObject(m_game_time_tips_title);
	SafeDeleteObject(m_battle_area);
	
	SafeDeleteArrayObject(m_score_string);
	SafeDeleteArrayObject(m_game_time_string);
	SafeDeleteArrayObject(m_score_add_string);
}

void GameScene::Init(HWND hWnd)
{
	/* ���洰�ھ�� */
	m_hWnd = hWnd;

	/* ���ó��������С */
	const int margin_x = 10;
	const int margin_y = 10;
	::GetClientRect(m_hWnd, &m_rect);

	// ���㳡���Ŀ�Ⱥ͸߶�
	m_scene_width = m_rect.right - m_rect.left;
	m_scene_height = m_rect.bottom - m_rect.top;

	// ������ͣ��ť�������С��λ��
	m_pause_button->SetFontSize(20);
	m_pause_button->SetX(m_rect.left + m_scene_width - m_pause_button->GetWidth());
	m_pause_button->SetY(0);

	// �������¿�ʼ��ť�������С
	m_new_game_button->SetFontSize(30);
	// �������˵���ť�������С
	m_switch_to_main_menu_button->SetFontSize(30);
	// �����˳���Ϸ��ť�������С
	m_exit_button->SetFontSize(30);

	// ������Ϸ��������������С
	m_game_over_title_tips->SetFontSize(35);
	// ������Ϸ����ԭ��������С
	m_game_over_reason_tips->SetFontSize(20);

	// ���÷���������ʾ���ı�ǰ��ɫ
	m_score_add_tips->SetForeground(RGB(251, 216, 96));
	// ������Ϸ����ԭ����ı�ǰ��ɫ
	m_game_over_reason_tips->SetForeground(RGB(0, 0, 0));
	// ���÷���������ı�ǰ��ɫ
	m_score_tips_title->SetForeground(RGB(0, 0, 0));
	// ������Ϸʱ�������ı�ǰ��ɫ
	m_game_time_tips_title->SetForeground(RGB(0, 0, 0));

	// ������ͣ��Ϸ��ť�ĵ��ί��
	m_pause_button->SetClickDelegate(bind(&GameScene::OnPuaseGameButtonClicked, this, _1, _2));
	// �������¿�ʼ��ť�ĵ��ί��
	m_new_game_button->SetClickDelegate(bind(&GameScene::OnNewGameButtonClicked, this, _1, _2));
	// �������˵���ť�ĵ��ί��
	m_switch_to_main_menu_button->SetClickDelegate(bind(&GameScene::OnSwitchMainMenuButtonClicked, this, _1, _2));
	// �����˳���Ϸ��ť�ĵ��ί��
	m_exit_button->SetClickDelegate(bind(&GameScene::OnExitGameButtonClicked, this, _1, _2));

	// ��Ϸ��ս���ľ���
	RECT rcBattle;
	// ��Ϸ��ս���ĸ��Ӵ�С
	const int nBattleHLatticeCount = 20, nBattleVLatticeCount = 20;
	memcpy((void*)&rcBattle, (void*)&m_rect, sizeof(m_rect));
	// ������Ϸ��ս���ľ���
	rcBattle.left += margin_x + (int)((m_scene_width % nBattleHLatticeCount) * .5f);
	rcBattle.right -= margin_x + (int)((m_scene_width % nBattleHLatticeCount) * .5f);
	rcBattle.top += margin_y + (int)((m_scene_height % nBattleVLatticeCount) * .5f);
	rcBattle.bottom	-= margin_y + (int)((m_scene_height % nBattleVLatticeCount) * .5f);
	rcBattle.top += 15;
	rcBattle.bottom += 15;

	// ������Ϸ��ս��
	m_battle_area = new GameBattleArea(rcBattle, nBattleHLatticeCount, nBattleVLatticeCount);
	// ��ʼ����Ϸ��ս��
	m_battle_area->Init(hWnd);
	// ����Ϸ������(GameScene)����Ϸ������������Ϸ��Ϸ��ս��(GameBattleArea)�ĳ�Ա����(function)������
	m_battle_area->fnGameOver = bind(&GameScene::OnGameOver, this);
	// ����Ϸ������(GameScene)�ķ������ӷ�������Ϸ��Ϸ��ս��(GameBattleArea)�ĳ�Ա����(function)������
	m_battle_area->fnBattleScoreAdd = bind(&GameScene::OnGameScoreAdd, this, _1);

	// ������Ϸ�������Ŀ�Ⱥ͸߶�
	m_game_over_panel_width = 300;
	m_game_over_panel_height = 250;

	// ��ʼ����Ϸ�����������ĵ�
	m_game_over_x_origin = m_rect.left + (int)(m_scene_width * .5f);
	m_game_over_y_origin = m_rect.top + (int)(m_scene_height * .5f);

	// ��ʼ����Ϸ�������ľ���
	m_game_over_panel_rect.left = m_game_over_x_origin - (int)(m_game_over_panel_width * .5f) - 3;
	m_game_over_panel_rect.right = m_game_over_x_origin + (int)(m_game_over_panel_width * .5f) + 3;
	m_game_over_panel_rect.top = m_game_over_y_origin - (int)(m_game_over_panel_height * .5f) - 3;
	m_game_over_panel_rect.bottom = m_game_over_y_origin + (int)(m_game_over_panel_height * .5f) + 3;

	// ������Ϸ��������"��Ϸ����"��ͷ��,���λ�ð�(û�й̶���,ֻҪ��������Ϸ�������ķ�Χ����).
	m_game_over_title_tips->SetX(m_game_over_x_origin - (int)(m_game_over_title_tips->GetWidth() * .5f));
	m_game_over_title_tips->SetY(m_game_over_y_origin - (int)(m_game_over_panel_height * .5f));
	
	// ������Ϸ������ԭ���y����λ��,Ҳ�Ǵ��λ�ð�.
	m_game_over_reason_tips->SetY(m_game_over_y_origin - (int)(m_game_over_panel_height * .5f) + m_game_over_title_tips->GetHeight() + 15);

	// �������¿�ʼ��ť��λ��,Ҳ�Ǵ��λ�ð�.
	m_new_game_button->SetX(m_game_over_x_origin - (int)(m_new_game_button->GetWidth() * .5f));
	m_new_game_button->SetY(m_game_over_reason_tips->GetY() + (int)(m_game_over_reason_tips->GetHeight() * .5f) + 35);

	// �������˵���ť��λ��,Ҳ�Ǵ��λ�ð�.
	m_switch_to_main_menu_button->SetX(m_game_over_x_origin - (int)(m_switch_to_main_menu_button->GetWidth() * .5f));
	m_switch_to_main_menu_button->SetY(m_new_game_button->GetY() + (int)(m_new_game_button->GetHeight() * .5f) + 35);

	// �����˳���Ϸ��ť��λ��,Ҳ�Ǵ��λ�ð�.
	m_exit_button->SetX(m_game_over_x_origin - (int)(m_exit_button->GetWidth() * .5f));
	m_exit_button->SetY(m_switch_to_main_menu_button->GetY() + (int)(m_switch_to_main_menu_button->GetHeight() * .5f) + 35);


	// ��ʼ����������Ϸʱ��
	m_score = 0;
	m_game_tick = 0;

	// ���÷����������С
	m_score_tips->SetFontSize(20);
	// ���÷���������ʾ�������С
	m_score_add_tips->SetFontSize(18);
	// ���÷�������������С
	m_score_tips_title->SetFontSize(20);
	// ������Ϸʱ��������С
	m_game_time_tips->SetFontSize(20);
	// ������Ϸʱ�����������С
	m_game_time_tips_title->SetFontSize(20);

	// ���÷�����x����λ��
	m_score_tips->SetX(m_score_tips_title->GetX() + m_score_tips_title->GetWidth());
	// ���÷���������ʾ��y����λ��
	m_score_add_tips->SetY(m_score_tips->GetY() + (int)((m_score_tips->GetHeight() - m_score_add_tips->GetHeight()) * .5f));
	// ������Ϸʱ������x����λ��
	m_game_time_tips_title->SetX((int)(m_rect.left + m_scene_width * .5f - m_game_time_tips_title->GetWidth() * .5f));
	// ������Ϸʱ���x����λ��
	m_game_time_tips->SetX(m_game_time_tips_title->GetX() + m_game_time_tips_title->GetWidth());
}

void GameScene::Render(HDC hdc)
{
	HBRUSH hGameBackgroundBrush = ::CreateSolidBrush(RGB(134, 71, 63));
	// ��Ⱦ��Ϸ�����ı���
	::FillRect(hdc, &m_rect, hGameBackgroundBrush);

	// ��Ⱦ��Ϸ��ս��
	m_battle_area->Render(hdc);
	// ��Ⱦ��ͣ��ť
	m_pause_button->Render(hdc);

	// ��Ⱦ����
	m_score_tips->Render(hdc);
	// ��Ⱦ��������
	m_score_tips_title->Render(hdc);
	// ��Ⱦ��Ϸʱ��
	m_game_time_tips->Render(hdc);
	// ��Ⱦ��Ϸʱ�����
	m_game_time_tips_title->Render(hdc);

	// ��Ⱦ��������
	m_score_add_tips->Render(hdc);

	// ��ս�Ƿ����
	if (m_battle_area->IsGameOver())
	{
		// ��Ⱦ��Ϸ����
		RenderGameOver(hdc);
	}

	SafeDeleteGDIObject(hGameBackgroundBrush);
}

void GameScene::Update(DWORD dwDeltaTime)
{
	// �������������ʾ�ɼ�,����ٿɼ���ʱ��,ֱ�������ʧ
	if (m_score_add_tips->IsVisible())
	{
		m_score_add_tips_time -= dwDeltaTime;
		if (m_score_add_tips_time <= 0)
		{
			m_score_add_tips->SetVisible(false);
		}
	}

	// ��Ϸ��ս���ĸ���
	m_battle_area->Update(dwDeltaTime);

	// ��ս�Ƿ����
	if (m_battle_area->IsGameOver())
		return;

	if (!m_battle_area->IsPause())
	{
		// �ۼ���Ϸʱ��(������)
		m_game_tick += dwDeltaTime;

		// ������ ת ʱ:��:��
		int hour = m_game_tick / 3600000;
		int min = m_game_tick % 3600000 / 60000;
		int second = m_game_tick % 3600000 % 60000 / 1000;

		// �������Ϸʱ����ַ���������
		_stprintf_s(m_game_time_string, 16, _T("%02d:%02d:%02d"), hour, min, second);
		// ������Ϸʱ��
		m_game_time_tips->SetText(m_game_time_string);
	}
}

void GameScene::OnKeyDown(Keys key)
{
	// ��Ϸ��ս���İ�������
	m_battle_area->OnKeyDown(key);
}

void GameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��ť�Դ�����Ϣ�Ĵ���
	m_pause_button->WndProc(hWnd, message, wParam, lParam);
	m_exit_button->WndProc(hWnd, message, wParam, lParam);

	m_new_game_button->WndProc(hWnd, message, wParam, lParam);
	m_switch_to_main_menu_button->WndProc(hWnd, message, wParam, lParam);
}

void GameScene::OnPuaseGameButtonClicked(int x, int y)
{
	// ��ͣ��ս
	if (!m_battle_area->IsPause())
		m_battle_area->Pause();
	else
		m_battle_area->Continue();

	m_pause_button->SetText(m_battle_area->IsPause() ? _T("������Ϸ") : _T("��ͣ��Ϸ"));
}

void GameScene::OnExitGameButtonClicked(int x, int y)
{
	// �����˳���Ϸ��ί�к���
	fnExitGame();
}

void GameScene::OnNewGameButtonClicked(int x, int y)
{
	// ��Ϸ��ս�������¿�ʼ
	m_battle_area->Restart(m_hWnd);

	// ������Ϸ����Ԫ��Ϊ���ɼ�
	m_game_over_title_tips->SetVisible(false);
	m_game_over_reason_tips->SetVisible(false);

	m_new_game_button->SetVisible(false);
	m_switch_to_main_menu_button->SetVisible(false);
	m_exit_button->SetVisible(false);

	// ������ͣ��Ϸ��ť
	m_pause_button->SetEnable(true);

	// ��ʼ����Ϸʱ��ͷ���
	m_game_tick = 0;
	m_score = 0;

	// ������������������ַ���������
	swprintf_s(m_score_string, 32, _T("%d"), m_score);
	// ���·���
	m_score_tips->SetText(m_score_string);
}

void GameScene::OnSwitchMainMenuButtonClicked(int x, int y)
{
	// �����л������˵���ί�к���
	fnSwitchToMainMenu();
}

void GameScene::OnGameOver()
{
	// ����Ϸ����ԭ��������ò�ͬ����Ϸ������ԭ��
	switch (m_battle_area->GetGameOverType())
	{
	case GOT_SELF: // ҧ���Լ�������Ϸ����.
		m_game_over_reason_tips->SetText(_T("��ҧ���Լ���!"));
		break;
	case GOT_TRAP: // ײ���ϱڵ�����Ϸ����.
		m_game_over_reason_tips->SetText(_T("��ײ���ϱ���!"));
		break;
	case GOT_WIN: // ����ﵽ�,��Ϸʤ��.
		m_game_over_reason_tips->SetText(_T("�ð���������ߴﵽ�����Ӯ�ˣ�"));
		break;
	default:
		break;
	}

	// ������Ϸ������ԭ��,Ҳ�Ǵ��λ�ð�.
	m_game_over_reason_tips->SetX(m_game_over_x_origin - (int)(m_game_over_reason_tips->GetWidth() * .5f));

	// ����"��Ϸ����"����Ŀɼ���
	m_game_over_title_tips->SetVisible(true);
	// ������Ϸ����ԭ��Ŀɼ���
	m_game_over_reason_tips->SetVisible(true);

	// �������¿�ʼ�����˵��Լ��˳���Ϸ��ť�Ŀɼ���
	m_new_game_button->SetVisible(true);
	m_switch_to_main_menu_button->SetVisible(true);
	m_exit_button->SetVisible(true);

	// ֹͣ������ͣ��ť
	m_pause_button->SetEnable(false);
}

void GameScene::OnGameScoreAdd(int score)
{
	// �ۼӷ���
	m_score += score;

	// ������������������ַ���������
	_stprintf_s(m_score_string, 32, _T("%d"), m_score);
	// ���·���
	m_score_tips->SetText(m_score_string);

	// ����������������������ַ���������
	_stprintf_s(m_score_add_string, 8, _T("+%d"), score);
	// ���·�������
	m_score_add_tips->SetText(m_score_add_string);
	// ���÷������ӵ�λ��
	m_score_add_tips->SetX(m_score_tips->GetX() + m_score_tips->GetWidth() + 5);
	// ���÷���������ʾ�Ŀɼ���
	m_score_add_tips->SetVisible(true);
	// ���÷����Ŀɼ�ʱ��
	m_score_add_tips_time = m_score_add_tips_visible_time;
}

void GameScene::RenderGameOver(HDC hdc)
{
	// ����Util��AlphaBlend ͸�����
	Util::AlphaBlend(hdc, m_rect.left, m_rect.top, m_scene_width, m_scene_height, RGB(0, 0, 0), 168);

	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(23, 23, 23));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	HBRUSH hBrush = (HBRUSH)::CreateSolidBrush(RGB(142, 53, 74));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// ���ƾ���(���߿��)
	::Rectangle(hdc, m_game_over_panel_rect.left, m_game_over_panel_rect.top, m_game_over_panel_rect.right, m_game_over_panel_rect.bottom);
	
	// ��Ⱦ��Ϸ��������
	m_game_over_title_tips->Render(hdc);
	// ��Ⱦ��Ϸ����ԭ��
	m_game_over_reason_tips->Render(hdc);
	// ��Ⱦ���¿�ʼ��ť
	m_new_game_button->Render(hdc);
	// ��Ⱦ���˵���ť
	m_switch_to_main_menu_button->Render(hdc);
	// ��Ⱦ�˳���Ϸ��ť
	m_exit_button->Render(hdc);

	::SelectObject(hdc, hOldBrush);
	::SelectObject(hdc, hOldPen);
	SafeDeleteGDIObject(hBrush);
	SafeDeleteGDIObject(hPen);
}

