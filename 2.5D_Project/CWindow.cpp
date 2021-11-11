#include "CWindow.h"
void CWindow::SetWindow(HWND hwnd)
{
	hwndGL = hwnd;
	InitWnd();
	hdc = GetDC(this->hwnd);
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

}
//输出文字
void CWindow::Textout(const TCHAR* str, int x, int y, int Length, COLORREF Color, string type, bool xieti)
{

	TextInfo* temp = new TextInfo;
	temp->color = Color;
	temp->str = new TCHAR[lstrlen(str) + 1];
	lstrcpy(temp->str, str);
	temp->size = Length;
	temp->x = x;
	temp->y = y;
	temp->type = type;
	temp->xieti = xieti;
	SendMessage(hwnd, WM_USER + 1, (WPARAM)temp, NULL);

	//SetTextColor(hdc, Color);
	//TextOut(hdc, x, y + cyTitle, str, lstrlen(str));
	//if (!stop)
	//	InvalidateRect(hwnd, NULL, true);
}
void CWindow::EquipmentTextout(const TCHAR* str, int x, int y, int Length, COLORREF Color, string type, bool xieti)
{
	TextInfo* temp = new TextInfo;
	temp->color = Color;
	temp->str = new TCHAR[lstrlen(str) + 1];
	lstrcpy(temp->str, str);
	temp->size = Length;
	temp->x = x;
	temp->y = y;
	temp->type = type;
	temp->xieti = xieti;
	SendMessage(hwnd, WM_USER + 3, (WPARAM)temp, NULL);
}
//暂停
void CWindow::Stop()
{
	SetFocus(hwndGL);
	if (start)
	{
		hContinue = CreateWindow(TEXT("Button"), TEXT("继续"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)1, hInstance, NULL);
		hExit = CreateWindow(TEXT("Button"), TEXT("退出"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)2, hInstance, NULL);
		hHelp = CreateWindow(TEXT("Button"), TEXT("帮助"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)4, hInstance, NULL);
		TCHAR temp[128] = TEXT("流畅度:");
		switch (SpeedLevel)
		{
		case 0:
			lstrcat(temp, TEXT("高"));
			break;
		case 1:
			lstrcat(temp, TEXT("中"));
			break;
		case 2:
			lstrcat(temp, TEXT("低"));
			break;
		}
		hLevel = CreateWindow(TEXT("Button"), temp, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)6, hInstance, NULL);
	}
	stop = true;
	//取消透明（这里的透明指接收不到鼠标消息
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
	
}
//坠机
void CWindow::Destroy()
{
	if (hRestart == NULL)
	{
		hRestart = CreateWindow(TEXT("Button"), TEXT("重新开始"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hwnd, (HMENU)3, hInstance, NULL);
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
	}
	//SetTextColor(hdc, RGB(0, 0, 0));
	//SetFont(100);
	RECT rect;
	GetClientRect(hwnd, &rect);
	TCHAR temp[256] = TEXT("飞机已坠毁 原因:");
	Textout(temp, 0, rect.bottom / 2 - 150, 100, 0);
	Textout(Cause, 0, rect.bottom / 2 - 50, 100, 0);
	if (stop == false)
		InvalidateRect(hwnd, NULL, true);
	//DrawText(hdc, temp, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	stop = true;
	fDestroy = true;
	//InvalidateRect(hwnd, NULL, true);
}
void CWindow::clear()
{
	SendMessage(hwnd, WM_USER + 2, NULL, NULL);
}
void CWindow::Equipmentclear()
{
	SendMessage(hwnd, WM_USER + 4, NULL, NULL);
}
void CWindow::Menuclear()
{
	SendMessage(hwnd, WM_USER + 6, NULL, NULL);
}
void CWindow::getmenu()
{
	GameMenu Menu_start[4] = {
	{ 1, TEXT("开始游戏"), 710, 600 },
	{ 2, TEXT("查看帮助"), 710, 650 },
	{ 3, TEXT("制作团队"), 710, 700 },
	{ 4, TEXT("退出游戏"), 710, 750 }
	};
	for (int i = 0; i < 4; i++)
	{
		GameMenu* temp = new GameMenu;
		temp->color = Menu_start[i].color;
		//temp->szName[i] = TCHAR[lstrlen(Menu_start[i].szName) + 1];
		lstrcpy(temp->szName, Menu_start[i].szName);
		//temp->size = 50;
		temp->xPos = Menu_start[i].xPos;
		temp->yPos = Menu_start[i].yPos;
		temp->type = Menu_start[i].type;
		//temp->xieti = true;
		SendMessage(hwnd, WM_USER + 5, (WPARAM)temp, NULL);
	}
}

//移动
void CWindow::Update()
{
	RECT GlRect;
	GetWindowRect(hwndGL, &GlRect);
	cxLength = GlRect.right - GlRect.left;
	cyLength = GlRect.bottom - GlRect.top;
	if (cxLength < cxScreen / 2 || cyLength < cyScreen / 2)
	{
		MoveWindow(hwndGL, GlRect.left, GlRect.top, cxScreen / 2, cyScreen / 2, true);
		cxLength = cxScreen / 2;
		cyLength = cyScreen / 2;
	}

	cxLength -= 20;
	cyLength -= 50;
	
	MoveWindow(hwnd, GlRect.left + 10, GlRect.top + 40, cxLength, cyLength, true);
	MoveWindow(hContinue, cxLength / 3, cyLength / 5, cxLength / 3, 50, true);
	MoveWindow(hHelp, cxLength / 3, cyLength * 2 / 5, cxLength / 3, 50, true);
	MoveWindow(hLevel, cxLength / 3, cyLength * 3 / 5, cxLength / 3, 50, true);
	MoveWindow(hExit, cxLength / 3, cyLength * 4 / 5, cxLength / 3, 50, true);
	MoveWindow(hRestart, cxLength / 3, cyLength * 2 / 3, cxLength / 3, 50, true);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CWindow* pWnd;
	static HWND hStatic, hReturn;
	static HINSTANCE hInstance;
	static int cxLength, cyLength;
	static HFONT hFont,hStaticFont;
	static TCHAR str;
	static vector<TextInfo> Texts;
	static vector<TextInfo> EquipmentTexts;
	static vector<GameMenu> MenuTexts;
	static bool noShowText = false;


	//int menuID = 0,oldmenuID=0;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;

	switch (message)
	{
	case WM_CREATE:
		hStaticFont = CreateFontA(40, 40, 0, 0, FW_THIN, false, false, false,
			CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_MODERN, "宋体");
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		SetLayeredWindowAttributes(hwnd, RGB(233, 133, 33), 0, LWA_COLORKEY);
		hStatic = CreateWindow(TEXT("static"), NULL, WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, NULL, hInstance, NULL);
		hReturn = CreateWindow(TEXT("button"), TEXT("返回"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hwnd, (HMENU)5, hInstance, NULL);
		SendMessage(hStatic, WM_SETFONT, (WPARAM)hStaticFont, true);
		SetTimer(hwnd, 0, 100, NULL);
		return 0;
	case WM_TIMER:
		//if (pWnd->stop || noShowText)
		//	return 0;
		
		InvalidateRect(hwnd, NULL, true);
		return 0;
	case WM_USER + 1:
		Texts.push_back(*(TextInfo*)wParam);
		return 0;
	case WM_USER + 2:
		Texts.clear();
		return 0;
	case WM_USER + 3:
		EquipmentTexts.push_back(*(TextInfo*)wParam);;
		return 0;
	case WM_USER + 4:
		EquipmentTexts.clear();
		return 0;
	case WM_USER + 5:
		MenuTexts.push_back(*(GameMenu*)wParam);
		return 0;
	case WM_USER + 6:
		MenuTexts.clear();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
	
		if (pWnd->menuID==0)
		{
			for (int i = 0; i < MenuTexts.size(); i++)
			{
				DeleteObject(hFont);
				hFont = CreateFontA(MenuTexts[i].size, MenuTexts[i].size, 0, 0, FW_THIN, MenuTexts[i].xieti, false, false,
					CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
					FF_MODERN, MenuTexts[i].type.c_str());
				SelectObject(hdc, hFont);
				MenuTexts[i].color = RGB(255, 255, 255);
				SetTextColor(hdc, MenuTexts[i].color);
				TextOut(hdc, MenuTexts[i].xPos, MenuTexts[i].yPos, MenuTexts[i].szName, lstrlen(MenuTexts[i].szName));
			}
		}
		else
		{
			for (int i = 0; i < MenuTexts.size(); i++)
			{
				DeleteObject(hFont);
				MenuTexts[i].size = 30;
				MenuTexts[pWnd->menuID - 1].size = 35;
				hFont = CreateFontA(MenuTexts[i].size, MenuTexts[i].size, 0, 0, FW_THIN, MenuTexts[i].xieti, false, false,
					CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
					FF_MODERN, MenuTexts[i].type.c_str());
				SelectObject(hdc, hFont);

				MenuTexts[i].color = RGB(255, 255, 255);
				MenuTexts[pWnd->menuID-1].color = RGB(1, 1, 1);
				SetTextColor(hdc, MenuTexts[i].color);
				TextOut(hdc, MenuTexts[i].xPos, MenuTexts[i].yPos, MenuTexts[i].szName, lstrlen(MenuTexts[i].szName));
			}
		}
		for (int i = 0; i < Texts.size(); i++)
		{
			DeleteObject(hFont);
			hFont = CreateFontA(Texts[i].size, Texts[i].size, 0, 0, FW_THIN, Texts[i].xieti, false, false,
				CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
				CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
				FF_MODERN, Texts[i].type.c_str());
			SelectObject(hdc, hFont);

			SetTextColor(hdc, Texts[i].color);
			TextOut(hdc, Texts[i].x, Texts[i].y, Texts[i].str, lstrlen(Texts[i].str));
		}

		for (int i = 0; i < EquipmentTexts.size(); i++)
		{
			DeleteObject(hFont);
			hFont = CreateFontA(EquipmentTexts[i].size, EquipmentTexts[i].size, 0, 0, FW_THIN, EquipmentTexts[i].xieti, false, false,
				CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
				CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
				FF_MODERN, EquipmentTexts[i].type.c_str());
			SelectObject(hdc, hFont);

			SetTextColor(hdc, EquipmentTexts[i].color);
			TextOut(hdc, EquipmentTexts[i].x, EquipmentTexts[i].y, EquipmentTexts[i].str, lstrlen(EquipmentTexts[i].str));
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_USER:
		pWnd = (CWindow*)lParam;
		return 0;
	case WM_MOUSEMOVE:
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &pt);

		if (pt.x < 710)	pWnd->menuID = 0;
		if (pt.x > 960) pWnd->menuID = 0;
		if (pt.y < 600)	pWnd->menuID = 0;
		if (pt.y > 750) pWnd->menuID = 0;

		if (pt.y > 595 && pt.y < 635 && pt.x >= 710 && pt.x <= 960)
			pWnd->menuID = 1;

		if (pt.y > 645 && pt.y < 685 && pt.x >= 710 && pt.x <= 960)
			pWnd->menuID = 2;

		if (pt.y > 695 && pt.y < 735 && pt.x >= 710 && pt.x <= 960)
			pWnd->menuID = 3;

		if (pt.y > 745 && pt.y < 785 && pt.x >= 710 && pt.x <= 960)
			pWnd->menuID = 4;
		break;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)CreateSolidBrush(RGB(187, 255, 255));
	case WM_LBUTTONDOWN:
		
		switch (pWnd->menuID)
		{
		case 1:
			//MessageBox(NULL, L"鼠标左键点击", L"Win32_Mouse", MB_OK);
			pWnd->start = true;
			pWnd->stop = false;
			pWnd->menuID = 0;
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
			break;
		case 2:
			exit(1);
			break;
		case 3:
			pWnd->menuID = 0;
			exit(1);
			break;
		case 4:
			pWnd->menuID = 0;
			exit(1);
			break;
		default:
			break;
		}
		SetFocus(pWnd->hwndGL);
		InvalidateRect(hwnd, NULL, true);
		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
			//MessageBox(0, 0, 0, 0);
			pWnd->stop = false;
			DestroyWindow(pWnd->hContinue);
			DestroyWindow(pWnd->hExit);
			DestroyWindow(pWnd->hHelp);
			DestroyWindow(pWnd->hLevel);
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
			break;
		case 2:
			exit(1);
			break;
		case 3:
			DestroyWindow(pWnd->hRestart);
			//pWnd->hRestart = NULL;
			pWnd->restart = true;
			InvalidateRect(hwnd, NULL, true);
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
			break;
		case 4:
			ShowWindow(pWnd->hContinue, SW_HIDE);
			ShowWindow(pWnd->hExit, SW_HIDE);
			ShowWindow(pWnd->hHelp, SW_HIDE);
			ShowWindow(pWnd->hLevel, SW_HIDE);
			InvalidateRect(hwnd, NULL, true);
			noShowText = true;

			ShowWindow(hStatic, SW_SHOW);
			ShowWindow(hReturn, SW_SHOW);
			MoveWindow(hStatic, 100, 100, cxLength - 200, cyLength - 200, true);
			MoveWindow(hReturn, cxLength - 300, 100, 200, 100, true);
			SetWindowText(hStatic, TEXT("使用说明:\n\n使用W键加速，S键减速\n\nQ键拉升，E键俯冲\n\nA键左转，D键右转\n\n"
				"飞机速度在达到200以上才可使用Q键拉升起飞\n\n使用A和D转向时会略微带动飞机向相应方向侧偏\n\n"
				"地图大小10w*10w\n\n在以下几种情况时降落会导致坠机\n\n1、滚转角超过30°\n\n2、速度超过500\n\n3、俯仰角低于-15°\n\n"
				"在飞行中速度小于200会强制下拉机头"));
			break;
		case 5:
			ShowWindow(pWnd->hContinue, SW_SHOW);
			ShowWindow(pWnd->hExit, SW_SHOW);
			ShowWindow(pWnd->hLevel, SW_SHOW);
			ShowWindow(pWnd->hHelp, SW_SHOW);
			ShowWindow(hStatic, SW_HIDE);
			ShowWindow(hReturn, SW_HIDE);
			noShowText = false;
			InvalidateRect(hwnd, NULL, true);
			break;

		case 6:
			TCHAR temp[128] = TEXT("流畅度:");
			switch (pWnd->SpeedLevel)
			{
			case 0:
				pWnd->SpeedLevel = 2;
				lstrcat(temp, TEXT("低"));
				break;
			case 1:
				pWnd->SpeedLevel = 0;
				lstrcat(temp, TEXT("高"));
				break;
			case 2:
				pWnd->SpeedLevel = 1;
				lstrcat(temp, TEXT("中"));
				break;
			}
			SetWindowText(pWnd->hLevel, temp);
			break;
		
		}
		SetFocus(pWnd->hwndGL);
		return 0;
	case WM_SIZE:
		cxLength = LOWORD(lParam);
		cyLength = HIWORD(lParam);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, 0);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
void MsgThread(LPVOID)
{
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void UpdateThread(LPVOID p)
{
	while (1)
	{
		((CWindow*)p)->Update();
		Sleep(20);
	}
}
void CWindow::InitWnd()
{
	hInstance = GetModuleHandle(NULL);
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT("Window");

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(RGB(233, 133, 33));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);

	hwnd = CreateWindowEx(
		WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED,
		szAppName,
		NULL,
		WS_POPUP,
		0, 0, 0, 0,
		hwndGL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	_beginthread(MsgThread, 0, NULL);
	_beginthread(UpdateThread, 0, (LPVOID)this);

	SendMessage(hwnd, WM_USER, 0, (LPARAM)this);
	SetFocus(hwndGL);
}
