
#include <windows.h>			// WIN32����������Ҫ��ͷ�ļ�(��������˰�...)

#include "GameType.h"			// ��Ϸ��ʹ�õ���һЩö�١���������
#include "Scene.h"				// ����������
#include "MainMenuScene.h"		// ���˵�������
#include "GameScene.h"			// ��Ϸ������
#include "Game.h"				// ��Ϸ��
#include "util.h"				// ���ù�����

// ����std�����ռ��е�bind
// std::bind ������(��ͨ�������������󡢳�Ա����)�󶨣�ͬʱ���ɵ��ö��󱣴�����(����Ҫ��ʱ�����)
// �󶨲����β�(��ռλ��, ����std::placeholders::_1��)
using std::bind;				
//using namespace std::placeholders; // ռλ��, �����std::bindʹ��

// ��ʼ����Ϸ��ľ�̬��Ա����, ���ھ��
// Ϊ�����ⲿ��ͨ������Game::GetGameHwnd()���л����Ϸ�Ĵ��ھ��
HWND Game::m_hWnd = NULL;

Game::Game() 
	: m_scene_flag(EnumGameScene::SCENE_MAINMENU), // Ĭ�ϳ���Ϊ���˵�
	m_is_switch_scene(false) // ��ʼ�����л�����
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd)
{
	// ���洰�ھ��
	m_hWnd = hWnd;
	// �л������˵�
	SwitchSceneToMainMenu();
}

void Game::Render(HDC hdc)
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	HBRUSH hBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	/* ��䱳����ɫ */
	::FillRect(hdc, &rect, hBrush);

	/* ���Ƴ��� */
	m_scene->Render(hdc);
}

void Game::Update(DWORD dwDeltaTime)
{
	// �Ƿ���Ҫ�л�����
	if (m_is_switch_scene)
	{
		// �����л��ĳ��������л���Ӧ�ĳ���
		switch (m_scene_flag)
		{
		case SCENE_MAINMENU:
			// �л������˵�����
			SwitchSceneToMainMenu();
			break;
		case SCENE_GAME:
			// �л�����Ϸ����
			SwitchSceneToGame();
			break;
		default:
			break;
		}
	}

	// ��������
	m_scene->Update(dwDeltaTime);
}

void Game::OnKeyDown(Keys key)
{
	// �����İ�������
	m_scene->OnKeyDown(key);
}

void Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��������Ϣ����
	m_scene->WndProc(hWnd, message, wParam, lParam);
}

void Game::SwitchSceneToMainMenu()
{
	// �Ƿ���ڳ���������������ͷŵ�ǰ��������Դ
	if (m_scene != NULL)
		SafeDeleteObject(m_scene);

	// ���˵�����
	MainMenuScene *scene;
	// ����һ�����˵�����
	m_scene = scene = new MainMenuScene();
	// ����Ϸ��(Game)���л�����Ϸ�������������˵�����(MainMenuScene)�ĳ�Ա����(function)������
	scene->fnSwitchToGame = bind(&Game::SetSwitchToGameFlag, this);
	// ����Ϸ��(Game)���˳���Ϸ���������˵�����(MainMenuScene)�ĳ�Ա����(function)������
	scene->fnExitGame = bind(&Game::ExitGame, this);
	// �������˵������ĳ�ʼ��
	m_scene->Init(m_hWnd);
	// ������Update�н����л��������
	m_is_switch_scene = false;
}

void Game::SwitchSceneToGame()
{
	// �Ƿ���ڳ���������������ͷŵ�ǰ��������Դ
	if (m_scene != NULL)
		SafeDeleteObject(m_scene);
	
	// ��Ϸ����
	GameScene *scene;
	// ����һ����Ϸ���������趨Ϊ��ǰ�ĳ���
	m_scene = scene = new GameScene();
	// ����Ϸ��(Game)���л��������泡����������Ϸ����(GameScene)�ĳ�Ա����(function)������
	scene->fnSwitchToMainMenu = bind(&Game::SetSwitchToMainMenuFlag, this);
	// ����Ϸ��(Game)���˳���Ϸ��������Ϸ����(GameScene)�ĳ�Ա����(function)������
	scene->fnExitGame = bind(&Game::ExitGame, this);
	// ������Ϸ�����ĳ�ʼ��
	m_scene->Init(m_hWnd);
	// ������Update�н����л��������
	m_is_switch_scene = false;
}

void Game::ExitGame()
{
	// SendMessage����WM_CLOSE(���ڹر���Ϣ),ʹ�ô��������ر�
	::SendMessage(Game::GetGameHwnd(), WM_CLOSE, 0, 0);
}

void Game::SetSwitchToMainMenuFlag()
{
	// �����л�����������һ��Update�߼������н����л�����
	m_is_switch_scene = true;
	// �����л��ĳ���
	m_scene_flag = EnumGameScene::SCENE_MAINMENU;
}

void Game::SetSwitchToGameFlag()
{
	// �����л�����������һ��Update�߼������н����л�����
	m_is_switch_scene = true;
	// �����л��ĳ���
	m_scene_flag = EnumGameScene::SCENE_GAME;
}
