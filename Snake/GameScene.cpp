
#include <windows.h>			// WIN32开发中最重要的头文件(不用详解了吧...)
#include <tchar.h>				// 通用字符集头文件,对于字符集之间的兼容比如ASCII编码和Unicode编码
#include <vector>				// vector容器所在头文件

#include "GameType.h"			// 游戏中使用到的一些枚举、别名定义
#include "Scene.h"				// 场景抽象类
#include "GameButton.h"			// 游戏按钮类
#include "GameLabel.h"			// 游戏标签类
#include "GameBattleArea.h"		// 游戏对战场类
#include "Util.h"				// 常用工具类
#include "GameScene.h"			// 游戏场景类

// Game.cpp 中已经介绍了,此处就不多说了.
using std::bind;
using namespace std::placeholders; // C++11新特征-占位符, 配合着std::bind使用

GameScene::GameScene() 
	: m_score_add_tips_visible_time(500L),					// 设置分数增提示的可见时间
	m_score_add_tips_time(m_score_add_tips_visible_time)	// 初始化分数增加提示当前已显示的时间
{
	// 创建按钮等.......(看名字哈)
	m_pause_button = new GameButton(0, 0, _T("暂停游戏"));
	m_new_game_button = new GameButton(0, 0, _T("重新开始"));
	m_switch_to_main_menu_button = new GameButton(0, 0, _T("主菜单"));
	m_exit_button = new GameButton(0, 0, _T("退出游戏"));

	// 创建标签等.......(看名字哈)
	m_game_over_title_tips = new GameLabel(0, 0, _T("游戏结束"));
	m_game_over_reason_tips = new GameLabel();
	m_score_tips = new GameLabel(0, 0, _T("0"));
	m_score_tips_title = new GameLabel(0, 0, _T("分数: "));
	m_score_add_tips = new GameLabel();
	m_game_time_tips = new GameLabel(0, 0, _T("00:00:00"));
	m_game_time_tips_title = new GameLabel(0, 0, _T("游戏时间: "));

	// 设置增加分数不可见
	m_score_add_tips->SetVisible(false);

	// 设置游戏结束提示等元素不可见
	m_game_over_title_tips->SetVisible(false);
	m_game_over_reason_tips->SetVisible(false);

	m_new_game_button->SetVisible(false);
	m_switch_to_main_menu_button->SetVisible(false);
	m_exit_button->SetVisible(false);

	// 为分数和游戏时间的字符串缓冲区分配内存
	m_score_string = new TCHAR[32];
	m_game_time_string = new TCHAR[16];
	m_score_add_string = new TCHAR[8];
}

GameScene::~GameScene()
{
	// 释放游戏场景的资源
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
	/* 保存窗口句柄 */
	m_hWnd = hWnd;

	/* 设置场景区域大小 */
	const int margin_x = 10;
	const int margin_y = 10;
	::GetClientRect(m_hWnd, &m_rect);

	// 计算场景的宽度和高度
	m_scene_width = m_rect.right - m_rect.left;
	m_scene_height = m_rect.bottom - m_rect.top;

	// 设置暂停按钮的字体大小和位置
	m_pause_button->SetFontSize(20);
	m_pause_button->SetX(m_rect.left + m_scene_width - m_pause_button->GetWidth());
	m_pause_button->SetY(0);

	// 设置重新开始按钮的字体大小
	m_new_game_button->SetFontSize(30);
	// 设置主菜单按钮的字体大小
	m_switch_to_main_menu_button->SetFontSize(30);
	// 设置退出游戏按钮的字体大小
	m_exit_button->SetFontSize(30);

	// 设置游戏结束标题的字体大小
	m_game_over_title_tips->SetFontSize(35);
	// 设置游戏结束原因的字体大小
	m_game_over_reason_tips->SetFontSize(20);

	// 设置分数增加提示的文本前景色
	m_score_add_tips->SetForeground(RGB(251, 216, 96));
	// 设置游戏结束原因的文本前景色
	m_game_over_reason_tips->SetForeground(RGB(0, 0, 0));
	// 设置分数标题的文本前景色
	m_score_tips_title->SetForeground(RGB(0, 0, 0));
	// 设置游戏时间标题的文本前景色
	m_game_time_tips_title->SetForeground(RGB(0, 0, 0));

	// 设置暂停游戏按钮的点击委托
	m_pause_button->SetClickDelegate(bind(&GameScene::OnPuaseGameButtonClicked, this, _1, _2));
	// 设置重新开始按钮的点击委托
	m_new_game_button->SetClickDelegate(bind(&GameScene::OnNewGameButtonClicked, this, _1, _2));
	// 设置主菜单按钮的点击委托
	m_switch_to_main_menu_button->SetClickDelegate(bind(&GameScene::OnSwitchMainMenuButtonClicked, this, _1, _2));
	// 设置退出游戏按钮的点击委托
	m_exit_button->SetClickDelegate(bind(&GameScene::OnExitGameButtonClicked, this, _1, _2));

	// 游戏对战场的矩形
	RECT rcBattle;
	// 游戏对战场的格子大小
	const int nBattleHLatticeCount = 20, nBattleVLatticeCount = 20;
	memcpy((void*)&rcBattle, (void*)&m_rect, sizeof(m_rect));
	// 计算游戏对战场的矩形
	rcBattle.left += margin_x + (int)((m_scene_width % nBattleHLatticeCount) * .5f);
	rcBattle.right -= margin_x + (int)((m_scene_width % nBattleHLatticeCount) * .5f);
	rcBattle.top += margin_y + (int)((m_scene_height % nBattleVLatticeCount) * .5f);
	rcBattle.bottom	-= margin_y + (int)((m_scene_height % nBattleVLatticeCount) * .5f);
	rcBattle.top += 15;
	rcBattle.bottom += 15;

	// 创建游戏对战场
	m_battle_area = new GameBattleArea(rcBattle, nBattleHLatticeCount, nBattleVLatticeCount);
	// 初始化游戏对战场
	m_battle_area->Init(hWnd);
	// 绑定游戏场景类(GameScene)的游戏结束方法到游戏游戏对战场(GameBattleArea)的成员函数(function)对象中
	m_battle_area->fnGameOver = bind(&GameScene::OnGameOver, this);
	// 绑定游戏场景类(GameScene)的分数增加方法到游戏游戏对战场(GameBattleArea)的成员函数(function)对象中
	m_battle_area->fnBattleScoreAdd = bind(&GameScene::OnGameScoreAdd, this, _1);

	// 设置游戏结束面板的宽度和高度
	m_game_over_panel_width = 300;
	m_game_over_panel_height = 250;

	// 初始化游戏结束面板的中心点
	m_game_over_x_origin = m_rect.left + (int)(m_scene_width * .5f);
	m_game_over_y_origin = m_rect.top + (int)(m_scene_height * .5f);

	// 初始化游戏结束面板的矩形
	m_game_over_panel_rect.left = m_game_over_x_origin - (int)(m_game_over_panel_width * .5f) - 3;
	m_game_over_panel_rect.right = m_game_over_x_origin + (int)(m_game_over_panel_width * .5f) + 3;
	m_game_over_panel_rect.top = m_game_over_y_origin - (int)(m_game_over_panel_height * .5f) - 3;
	m_game_over_panel_rect.bottom = m_game_over_y_origin + (int)(m_game_over_panel_height * .5f) + 3;

	// 设置游戏结束标题"游戏结束"在头顶,大概位置吧(没有固定的,只要不超过游戏结束面板的范围即可).
	m_game_over_title_tips->SetX(m_game_over_x_origin - (int)(m_game_over_title_tips->GetWidth() * .5f));
	m_game_over_title_tips->SetY(m_game_over_y_origin - (int)(m_game_over_panel_height * .5f));
	
	// 设置游戏结束的原因的y坐标位置,也是大概位置吧.
	m_game_over_reason_tips->SetY(m_game_over_y_origin - (int)(m_game_over_panel_height * .5f) + m_game_over_title_tips->GetHeight() + 15);

	// 设置重新开始按钮的位置,也是大概位置吧.
	m_new_game_button->SetX(m_game_over_x_origin - (int)(m_new_game_button->GetWidth() * .5f));
	m_new_game_button->SetY(m_game_over_reason_tips->GetY() + (int)(m_game_over_reason_tips->GetHeight() * .5f) + 35);

	// 设置主菜单按钮的位置,也是大概位置吧.
	m_switch_to_main_menu_button->SetX(m_game_over_x_origin - (int)(m_switch_to_main_menu_button->GetWidth() * .5f));
	m_switch_to_main_menu_button->SetY(m_new_game_button->GetY() + (int)(m_new_game_button->GetHeight() * .5f) + 35);

	// 设置退出游戏按钮的位置,也是大概位置吧.
	m_exit_button->SetX(m_game_over_x_origin - (int)(m_exit_button->GetWidth() * .5f));
	m_exit_button->SetY(m_switch_to_main_menu_button->GetY() + (int)(m_switch_to_main_menu_button->GetHeight() * .5f) + 35);


	// 初始化分数和游戏时间
	m_score = 0;
	m_game_tick = 0;

	// 设置分数的字体大小
	m_score_tips->SetFontSize(20);
	// 设置分数增加提示的字体大小
	m_score_add_tips->SetFontSize(18);
	// 设置分数标题的字体大小
	m_score_tips_title->SetFontSize(20);
	// 设置游戏时间的字体大小
	m_game_time_tips->SetFontSize(20);
	// 设置游戏时间标题的字体大小
	m_game_time_tips_title->SetFontSize(20);

	// 设置分数的x坐标位置
	m_score_tips->SetX(m_score_tips_title->GetX() + m_score_tips_title->GetWidth());
	// 设置分数增加提示的y坐标位置
	m_score_add_tips->SetY(m_score_tips->GetY() + (int)((m_score_tips->GetHeight() - m_score_add_tips->GetHeight()) * .5f));
	// 设置游戏时间标题的x坐标位置
	m_game_time_tips_title->SetX((int)(m_rect.left + m_scene_width * .5f - m_game_time_tips_title->GetWidth() * .5f));
	// 设置游戏时间的x坐标位置
	m_game_time_tips->SetX(m_game_time_tips_title->GetX() + m_game_time_tips_title->GetWidth());
}

void GameScene::Render(HDC hdc)
{
	HBRUSH hGameBackgroundBrush = ::CreateSolidBrush(RGB(134, 71, 63));
	// 渲染游戏场景的背景
	::FillRect(hdc, &m_rect, hGameBackgroundBrush);

	// 渲染游戏对战场
	m_battle_area->Render(hdc);
	// 渲染暂停按钮
	m_pause_button->Render(hdc);

	// 渲染分数
	m_score_tips->Render(hdc);
	// 渲染分数标题
	m_score_tips_title->Render(hdc);
	// 渲染游戏时间
	m_game_time_tips->Render(hdc);
	// 渲染游戏时间标题
	m_game_time_tips_title->Render(hdc);

	// 渲染分数增加
	m_score_add_tips->Render(hdc);

	// 对战是否结束
	if (m_battle_area->IsGameOver())
	{
		// 渲染游戏结束
		RenderGameOver(hdc);
	}

	SafeDeleteGDIObject(hGameBackgroundBrush);
}

void GameScene::Update(DWORD dwDeltaTime)
{
	// 如果分数增加提示可见,则减少可见的时间,直到最后消失
	if (m_score_add_tips->IsVisible())
	{
		m_score_add_tips_time -= dwDeltaTime;
		if (m_score_add_tips_time <= 0)
		{
			m_score_add_tips->SetVisible(false);
		}
	}

	// 游戏对战场的更新
	m_battle_area->Update(dwDeltaTime);

	// 对战是否结束
	if (m_battle_area->IsGameOver())
		return;

	if (!m_battle_area->IsPause())
	{
		// 累加游戏时间(毫秒数)
		m_game_tick += dwDeltaTime;

		// 毫秒数 转 时:分:秒
		int hour = m_game_tick / 3600000;
		int min = m_game_tick % 3600000 / 60000;
		int second = m_game_tick % 3600000 % 60000 / 1000;

		// 输出到游戏时间的字符串缓冲区
		_stprintf_s(m_game_time_string, 16, _T("%02d:%02d:%02d"), hour, min, second);
		// 更新游戏时间
		m_game_time_tips->SetText(m_game_time_string);
	}
}

void GameScene::OnKeyDown(Keys key)
{
	// 游戏对战场的按键处理
	m_battle_area->OnKeyDown(key);
}

void GameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 按钮对窗口消息的处理
	m_pause_button->WndProc(hWnd, message, wParam, lParam);
	m_exit_button->WndProc(hWnd, message, wParam, lParam);

	m_new_game_button->WndProc(hWnd, message, wParam, lParam);
	m_switch_to_main_menu_button->WndProc(hWnd, message, wParam, lParam);
}

void GameScene::OnPuaseGameButtonClicked(int x, int y)
{
	// 暂停对战
	if (!m_battle_area->IsPause())
		m_battle_area->Pause();
	else
		m_battle_area->Continue();

	m_pause_button->SetText(m_battle_area->IsPause() ? _T("继续游戏") : _T("暂停游戏"));
}

void GameScene::OnExitGameButtonClicked(int x, int y)
{
	// 调用退出游戏的委托函数
	fnExitGame();
}

void GameScene::OnNewGameButtonClicked(int x, int y)
{
	// 游戏对战场的重新开始
	m_battle_area->Restart(m_hWnd);

	// 设置游戏结束元素为不可见
	m_game_over_title_tips->SetVisible(false);
	m_game_over_reason_tips->SetVisible(false);

	m_new_game_button->SetVisible(false);
	m_switch_to_main_menu_button->SetVisible(false);
	m_exit_button->SetVisible(false);

	// 启用暂停游戏按钮
	m_pause_button->SetEnable(true);

	// 初始化游戏时间和分数
	m_game_tick = 0;
	m_score = 0;

	// 将分数输出到分数的字符串缓冲区
	swprintf_s(m_score_string, 32, _T("%d"), m_score);
	// 更新分数
	m_score_tips->SetText(m_score_string);
}

void GameScene::OnSwitchMainMenuButtonClicked(int x, int y)
{
	// 调用切换到主菜单的委托函数
	fnSwitchToMainMenu();
}

void GameScene::OnGameOver()
{
	// 对游戏结束原因进行设置不同的游戏结束的原因
	switch (m_battle_area->GetGameOverType())
	{
	case GOT_SELF: // 咬到自己导致游戏结束.
		m_game_over_reason_tips->SetText(_T("你咬到自己啦!"));
		break;
	case GOT_TRAP: // 撞到障壁导致游戏结束.
		m_game_over_reason_tips->SetText(_T("你撞到障壁了!"));
		break;
	case GOT_WIN: // 蛇身达到最长,游戏胜利.
		m_game_over_reason_tips->SetText(_T("好棒，你的蛇蛇达到最长，你赢了！"));
		break;
	default:
		break;
	}

	// 设置游戏结束的原因,也是大概位置吧.
	m_game_over_reason_tips->SetX(m_game_over_x_origin - (int)(m_game_over_reason_tips->GetWidth() * .5f));

	// 设置"游戏结束"标题的可见性
	m_game_over_title_tips->SetVisible(true);
	// 设置游戏结束原因的可见性
	m_game_over_reason_tips->SetVisible(true);

	// 设置重新开始和主菜单以及退出游戏按钮的可见性
	m_new_game_button->SetVisible(true);
	m_switch_to_main_menu_button->SetVisible(true);
	m_exit_button->SetVisible(true);

	// 停止启用暂停按钮
	m_pause_button->SetEnable(false);
}

void GameScene::OnGameScoreAdd(int score)
{
	// 累加分数
	m_score += score;

	// 将分数输出到分数的字符串缓冲区
	_stprintf_s(m_score_string, 32, _T("%d"), m_score);
	// 更新分数
	m_score_tips->SetText(m_score_string);

	// 将分数增加输出到分数的字符串缓冲区
	_stprintf_s(m_score_add_string, 8, _T("+%d"), score);
	// 更新分数增加
	m_score_add_tips->SetText(m_score_add_string);
	// 设置分数增加的位置
	m_score_add_tips->SetX(m_score_tips->GetX() + m_score_tips->GetWidth() + 5);
	// 设置分数增加提示的可见性
	m_score_add_tips->SetVisible(true);
	// 设置分数的可见时间
	m_score_add_tips_time = m_score_add_tips_visible_time;
}

void GameScene::RenderGameOver(HDC hdc)
{
	// 调用Util的AlphaBlend 透明混合
	Util::AlphaBlend(hdc, m_rect.left, m_rect.top, m_scene_width, m_scene_height, RGB(0, 0, 0), 168);

	HPEN hPen = ::CreatePen(PS_SOLID, 3, RGB(23, 23, 23));
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
	HBRUSH hBrush = (HBRUSH)::CreateSolidBrush(RGB(142, 53, 74));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

	// 绘制矩形(带边框的)
	::Rectangle(hdc, m_game_over_panel_rect.left, m_game_over_panel_rect.top, m_game_over_panel_rect.right, m_game_over_panel_rect.bottom);
	
	// 渲染游戏结束标题
	m_game_over_title_tips->Render(hdc);
	// 渲染游戏结束原因
	m_game_over_reason_tips->Render(hdc);
	// 渲染重新开始按钮
	m_new_game_button->Render(hdc);
	// 渲染主菜单按钮
	m_switch_to_main_menu_button->Render(hdc);
	// 渲染退出游戏按钮
	m_exit_button->Render(hdc);

	::SelectObject(hdc, hOldBrush);
	::SelectObject(hdc, hOldPen);
	SafeDeleteGDIObject(hBrush);
	SafeDeleteGDIObject(hPen);
}

