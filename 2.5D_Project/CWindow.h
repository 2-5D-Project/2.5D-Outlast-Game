#pragma once

#include <Windows.h>
#include <process.h>
#include <iostream>
#include <vector>
using namespace std;

//输出的字符串的基本信息
struct TextInfo
{
	int size;
	int x;
	int y;
	COLORREF color;
	TCHAR* str;
	string type;
	bool xieti;
};

struct GameMenu
{
	int id;//按钮ID
	TCHAR szName[10];//按钮名称
	int xPos;//距离客户区左边的位置
	int yPos;//距离客户区顶部的位置
	COLORREF color=RGB(255,255,255);
	string type = "宋体";
	int size=30;
	bool xieti=true;
};

class CWindow
{
public:
	
	void SetWindow(HWND hwnd);
	//这边可以自定义字体的一些信息
	void Textout(const TCHAR* str, int x, int y, int Length, COLORREF Color = RGB(255, 1, 255), string type = "宋体", bool xieti = true);
	void EquipmentTextout(const TCHAR* str, int x, int y, int Length, COLORREF Color = RGB(255, 255, 255), string type = "宋体", bool xieti = true);
	//暂停
	void Stop();
	//坠机
	void Destroy();
	//清空储存TextInfo的容器
	void clear();
	void Equipmentclear();
	void Menuclear();
	void getmenu();

	int menuID = 0, oldmenuID = 0;
	HWND Menu;
	bool start = false;
	bool fDestroy = false;
	bool stop = false;
	bool restart = false;
	//流畅度 0为高 2为低
	int SpeedLevel = 1;
	//坠机原因
	TCHAR* Cause;
	//重新开始按钮
	HWND hRestart = NULL;
private:
	//此窗口和OpenGL的窗口句柄
	HWND hwnd, hwndGL;
	//按钮句柄
	HWND hContinue, hExit, hHelp, hLevel;
	HDC hdc;
	HINSTANCE hInstance;
	//窗口长宽
	int cxLength, cyLength;
	//屏幕长宽
	int cxScreen, cyScreen;

	void InitWnd();
	void Update();

	//窗口消息响应函数
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//将此窗口移动到和OpenGL的窗口重合的位置
	friend void UpdateThread(LPVOID p);
};

