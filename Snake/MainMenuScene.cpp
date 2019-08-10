
#include <windows.h>			// WIN32开发中最重要的头文件(不用详解了吧...)
#include <tchar.h>				// 通用字符集头文件,对于字符集之间的兼容比如ASCII编码和Unicode编码

#include "GameType.h"			// 游戏中使用到的一些枚举、别名定义
#include "Scene.h"				// 游戏场景抽象类
#include "Game.h"				// 游戏类
#include "GameButton.h"			// 游戏按钮类
#include "GameLabel.h"			// 游戏标签类
#include "util.h"				// 常用工具类
#include "MainMenuScene.h"		// 主菜单场景类

MainMenuScene::MainMenuScene()
{
	// 创建开始游戏按钮等.......(看名字哈)
	m_start_game_button = new GameButton(0, 0, _T("开始游戏"));
	m_exit_game_button = new GameButton(0, 0, _T("退出游戏"));
	// 这里这里, 用了三个Label进行表示 "贪"、"吃"、"蛇" 只是为了设置三个字不同的颜色效果。(｡♥ᴗ♥｡) 
	m_title_first_word_label = new GameLabel(0, 0, _T("贪"));
	m_title_second_word_label = new GameLabel(0, 0, _T("吃"));
	m_title_third_word_label = new GameLabel(0, 0, _T("蛇"));
}

MainMenuScene::~MainMenuScene()
{
	// 释放主界面场景的资源
	SafeDeleteObject(m_start_game_button);
	SafeDeleteObject(m_exit_game_button);
	SafeDeleteObject(m_title_first_word_label);
	SafeDeleteObject(m_title_second_word_label);
	SafeDeleteObject(m_title_third_word_label);
}

void MainMenuScene::Init(HWND hWnd)
{
	// 获取窗口的矩形
	::GetClientRect(hWnd, &m_rect_scene);

	// 获取场景的宽度和高度
	int scene_width = m_rect_scene.right - m_rect_scene.left;
	int scene_height = m_rect_scene.bottom - m_rect_scene.top;
	// 获取场景的中心坐标
	float x_center = m_rect_scene.left + scene_width * .5f;
	float y_center = m_rect_scene.top + scene_height * .5f;

	// 设置开始游戏按钮的字体大小，位置
	m_start_game_button->SetFontSize(50);
	m_start_game_button->SetX((int)(x_center - m_start_game_button->GetWidth() * .5f));
	m_start_game_button->SetY((int)(y_center - m_start_game_button->GetHeight() * .5f - scene_height * .1f));
	
	// 设置退出游戏按钮的字体大小，位置
	m_exit_game_button->SetFontSize(50);
	m_exit_game_button->SetX((int)(x_center - m_exit_game_button->GetWidth() * .5f));
	m_exit_game_button->SetY((int)(y_center - m_exit_game_button->GetHeight() * .5f + scene_height * .1f));
	
	// 设置贪吃蛇的"贪"标签的字体大小
	m_title_first_word_label->SetFontSize(125);
	// 设置贪吃蛇的"吃"标签的字体大小
	m_title_second_word_label->SetFontSize(125);
	// 设置贪吃蛇的"蛇"标签的字体大小
	m_title_third_word_label->SetFontSize(125);

	// 设置贪吃蛇的"贪"标签的前景色
	m_title_first_word_label->SetForeground(RGB(238, 130, 238));
	// 设置贪吃蛇的"吃"标签的前景色
	m_title_second_word_label->SetForeground(RGB(218, 112, 214));
	// 设置贪吃蛇的"蛇"标签的前景色
	m_title_third_word_label->SetForeground(RGB(255, 105, 180));

	// 设置贪吃蛇的"吃"标签的x坐标位置, 先设定中间的 "吃" 字为中心, "贪"和"蛇"则根据他的位置设定为其左右
	m_title_second_word_label->SetX((int)(x_center - m_title_second_word_label->GetWidth() * .5f));
	// 设置贪吃蛇的"贪"标签的x坐标位置, 位置为"吃"的左边
	m_title_first_word_label->SetX(m_title_second_word_label->GetX() - m_title_first_word_label->GetWidth());
	// 设置贪吃蛇的"蛇"标签的x坐标位置, 位置为"吃"的右边
	m_title_third_word_label->SetX(m_title_second_word_label->GetX() + m_title_third_word_label->GetWidth());

	// 设置贪吃蛇的"贪"标签的y坐标位置
	m_title_first_word_label->SetY((int)(scene_height * .05f));
	// 设置贪吃蛇的"吃"标签的y坐标位置
	m_title_second_word_label->SetY(m_title_first_word_label->GetY());
	// 设置贪吃蛇的"蛇"标签的y坐标位置
	m_title_third_word_label->SetY(m_title_first_word_label->GetY());

	// 设置开始游戏按钮点击时候的委托
	m_start_game_button->SetClickDelegate(
		std::bind(&MainMenuScene::OnStartGameButtonClicked, this, 
			std::placeholders::_1, std::placeholders::_2));
	// 设置退出游戏按钮点击时候的委托
	m_exit_game_button->SetClickDelegate(
		std::bind(&MainMenuScene::OnExitGameButtonClicked, this, 
			std::placeholders::_1, std::placeholders::_2));
}

void MainMenuScene::Render(HDC hdc)
{
	// 填充场景背景
	::FillRect(hdc, &m_rect_scene, ::CreateSolidBrush(RGB(134, 71, 63)));
	//渲染开始游戏按钮
	m_start_game_button->Render(hdc);
	// 渲染退出游戏按钮
	m_exit_game_button->Render(hdc);
	// 渲染贪吃蛇的"贪"标签
	m_title_first_word_label->Render(hdc);
	// 渲染贪吃蛇的"吃"标签
	m_title_second_word_label->Render(hdc);
	// 渲染贪吃蛇的"蛇"标签
	m_title_third_word_label->Render(hdc);
}

void MainMenuScene::Update(DWORD dwDeltaTime)
{
	// 更新主菜单场景
}

void MainMenuScene::OnKeyDown(Keys key)
{
	// 主菜单场景的按键处理
}

void MainMenuScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 主菜单场景对窗口消息的处理

	// 开始游戏按钮对窗口消息的处理
	m_start_game_button->WndProc(hWnd, message, wParam, lParam);
	// 退出游戏按钮对窗口消息的处理
	m_exit_game_button->WndProc(hWnd, message, wParam, lParam);
}

void MainMenuScene::OnStartGameButtonClicked(int x, int y)
{
	// 调用切换到游戏场景的委托函数
	fnSwitchToGame();
}

void MainMenuScene::OnExitGameButtonClicked(int x, int y)
{
	// 调用退出游戏的委托函数
	fnExitGame();
}
