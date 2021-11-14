#pragma once

#include <Windows.h>
#include <process.h>
#include <iostream>
#include <vector>
using namespace std;

//������ַ����Ļ�����Ϣ
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
	int id;//��ťID
	TCHAR szName[10];//��ť����
	int xPos;//����ͻ�����ߵ�λ��
	int yPos;//����ͻ���������λ��
	COLORREF color=RGB(255,255,255);
	string type = "����";
	int size=30;
	bool xieti=true;
};

class CWindow
{
public:
	
	void SetWindow(HWND hwnd);
	//��߿����Զ��������һЩ��Ϣ
	void Textout(const TCHAR* str, int x, int y, int Length, COLORREF Color = RGB(255, 1, 255), string type = "����", bool xieti = true);
	void EquipmentTextout(const TCHAR* str, int x, int y, int Length, COLORREF Color = RGB(255, 255, 255), string type = "����", bool xieti = true);
	//��ͣ
	void Stop();
	//׹��
	void Destroy();
	//��մ���TextInfo������
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
	//������ 0Ϊ�� 2Ϊ��
	int SpeedLevel = 1;
	//׹��ԭ��
	TCHAR* Cause;
	//���¿�ʼ��ť
	HWND hRestart = NULL;
private:
	//�˴��ں�OpenGL�Ĵ��ھ��
	HWND hwnd, hwndGL;
	//��ť���
	HWND hContinue, hExit, hHelp, hLevel;
	HDC hdc;
	HINSTANCE hInstance;
	//���ڳ���
	int cxLength, cyLength;
	//��Ļ����
	int cxScreen, cyScreen;

	void InitWnd();
	void Update();

	//������Ϣ��Ӧ����
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//���˴����ƶ�����OpenGL�Ĵ����غϵ�λ��
	friend void UpdateThread(LPVOID p);
};

