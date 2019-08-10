
#include <windows.h>			// WIN32开发中最重要的头文件(不用详解了吧...)

#include "GameType.h"			// 游戏中使用到的一些枚举、别名定义
#include "Scene.h"				// 场景抽象类
#include "MainMenuScene.h"		// 主菜单场景类
#include "GameScene.h"			// 游戏场景类
#include "Game.h"				// 游戏类
#include "util.h"				// 常用工具类

// 引入std命名空间中的bind
// std::bind 将函数(普通函数、函数对象、成员函数)绑定，同时将可调用对象保存起来(在需要的时候调用)
// 绑定部分形参(或占位符, 例如std::placeholders::_1等)
using std::bind;				
//using namespace std::placeholders; // 占位符, 配合着std::bind使用

// 初始化游戏类的静态成员变量, 窗口句柄
// 为了在外部能通过调用Game::GetGameHwnd()进行获得游戏的窗口句柄
HWND Game::m_hWnd = NULL;

Game::Game() 
	: m_scene_flag(EnumGameScene::SCENE_MAINMENU), // 默认场景为主菜单
	m_is_switch_scene(false) // 初始化不切换场景
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd)
{
	// 保存窗口句柄
	m_hWnd = hWnd;
	// 切换到主菜单
	SwitchSceneToMainMenu();
}

void Game::Render(HDC hdc)
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	HBRUSH hBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	/* 填充背景颜色 */
	::FillRect(hdc, &rect, hBrush);

	/* 绘制场景 */
	m_scene->Render(hdc);
}

void Game::Update(DWORD dwDeltaTime)
{
	// 是否需要切换场景
	if (m_is_switch_scene)
	{
		// 根据切换的场景进行切换相应的场景
		switch (m_scene_flag)
		{
		case SCENE_MAINMENU:
			// 切换到主菜单场景
			SwitchSceneToMainMenu();
			break;
		case SCENE_GAME:
			// 切换到游戏场景
			SwitchSceneToGame();
			break;
		default:
			break;
		}
	}

	// 场景更新
	m_scene->Update(dwDeltaTime);
}

void Game::OnKeyDown(Keys key)
{
	// 场景的按键处理
	m_scene->OnKeyDown(key);
}

void Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 场景的消息处理
	m_scene->WndProc(hWnd, message, wParam, lParam);
}

void Game::SwitchSceneToMainMenu()
{
	// 是否存在场景，如果存在则释放当前场景的资源
	if (m_scene != NULL)
		SafeDeleteObject(m_scene);

	// 主菜单场景
	MainMenuScene *scene;
	// 创建一个主菜单场景
	m_scene = scene = new MainMenuScene();
	// 绑定游戏类(Game)的切换到游戏场景方法到主菜单场景(MainMenuScene)的成员函数(function)对象中
	scene->fnSwitchToGame = bind(&Game::SetSwitchToGameFlag, this);
	// 绑定游戏类(Game)的退出游戏方法到主菜单场景(MainMenuScene)的成员函数(function)对象中
	scene->fnExitGame = bind(&Game::ExitGame, this);
	// 调用主菜单场景的初始化
	m_scene->Init(m_hWnd);
	// 设置在Update中进行切换场景完成
	m_is_switch_scene = false;
}

void Game::SwitchSceneToGame()
{
	// 是否存在场景，如果存在则释放当前场景的资源
	if (m_scene != NULL)
		SafeDeleteObject(m_scene);
	
	// 游戏场景
	GameScene *scene;
	// 创建一个游戏场景，并设定为当前的场景
	m_scene = scene = new GameScene();
	// 绑定游戏类(Game)的切换到主界面场景方法到游戏场景(GameScene)的成员函数(function)对象中
	scene->fnSwitchToMainMenu = bind(&Game::SetSwitchToMainMenuFlag, this);
	// 绑定游戏类(Game)的退出游戏方法到游戏场景(GameScene)的成员函数(function)对象中
	scene->fnExitGame = bind(&Game::ExitGame, this);
	// 调用游戏场景的初始化
	m_scene->Init(m_hWnd);
	// 设置在Update中进行切换场景完成
	m_is_switch_scene = false;
}

void Game::ExitGame()
{
	// SendMessage发送WM_CLOSE(窗口关闭消息),使得窗口正常关闭
	::SendMessage(Game::GetGameHwnd(), WM_CLOSE, 0, 0);
}

void Game::SetSwitchToMainMenuFlag()
{
	// 设置切换场景，在下一个Update逻辑更新中进行切换场景
	m_is_switch_scene = true;
	// 设置切换的场景
	m_scene_flag = EnumGameScene::SCENE_MAINMENU;
}

void Game::SetSwitchToGameFlag()
{
	// 设置切换场景，在下一个Update逻辑更新中进行切换场景
	m_is_switch_scene = true;
	// 设置切换的场景
	m_scene_flag = EnumGameScene::SCENE_GAME;
}
