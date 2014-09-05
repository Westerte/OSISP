// Win32Project1.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100
#define CREATE 1
#define OPENF 2
#define SAVEF 3
#define SAVEFA 4
#define PRINT 5
#define ABOUT 6
#define EX 7

// ���������� ����������:
HWND ItemsBar;
HWND DrawArea;
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����
TCHAR szMainBar[MAX_LOADSTRING] = _T("MainBar");


// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM            RegisterItemsBar(HINSTANCE hInstance);
ATOM            RegisterDrawingWind(HINSTANCE hInstance);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DrawProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ItemsBarProc(HWND, UINT, WPARAM, LPARAM);

HMENU MainBar();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	RegisterItemsBar(hInstance);
	RegisterDrawingWind(hInstance);
	MyRegisterClass(hInstance);
	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// ���� ��������� ���������:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM RegisterDrawingWind(HINSTANCE hInstance)
{
	WNDCLASSEX DrawArea;

	DrawArea.cbSize = sizeof(WNDCLASSEX);
	DrawArea.style = CS_HREDRAW | CS_VREDRAW;
	DrawArea.lpfnWndProc = DrawProc;
	DrawArea.cbClsExtra = 0;
	DrawArea.cbWndExtra = 0;
	DrawArea.hInstance = hInstance;
	DrawArea.hIcon = NULL;
	DrawArea.hCursor = LoadCursor(NULL, IDC_ARROW);
	DrawArea.hbrBackground = (HBRUSH)(6);
	DrawArea.lpszMenuName = NULL;
	DrawArea.lpszClassName = _T("DrawAria");
	DrawArea.hIconSm = NULL;

	return RegisterClassEx(&DrawArea);
}

ATOM RegisterItemsBar(HINSTANCE hInstance)
{
	WNDCLASSEX ItemsBar;

	ItemsBar.cbSize = sizeof(WNDCLASSEX);
	ItemsBar.style = CS_HREDRAW | CS_VREDRAW;
	ItemsBar.lpfnWndProc = ItemsBarProc;
	ItemsBar.cbClsExtra = 0;
	ItemsBar.cbWndExtra = 0;
	ItemsBar.hInstance = hInstance;
	ItemsBar.hIcon = NULL;
	ItemsBar.hCursor = LoadCursor(NULL, IDC_ARROW);
	ItemsBar.hbrBackground = CreateSolidBrush(RGB(251, 254, 255));
	ItemsBar.lpszMenuName = NULL;
	ItemsBar.lpszClassName = _T("ItemsBar");
	ItemsBar.hIconSm = NULL;

	return RegisterClassEx(&ItemsBar);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX MainPW;

	MainPW.cbSize = sizeof(WNDCLASSEX);

	MainPW.style = CS_HREDRAW | CS_VREDRAW;
	MainPW.lpfnWndProc = WndProc;
	MainPW.cbClsExtra = 0;
	MainPW.cbWndExtra = 0;
	MainPW.hInstance = hInstance;
	MainPW.hIcon = NULL;
	MainPW.hCursor = LoadCursor(NULL, IDC_ARROW);
	MainPW.hbrBackground = (HBRUSH)(29);
	MainPW.lpszMenuName = NULL;
	MainPW.lpszClassName = szWindowClass;
	MainPW.hIconSm = NULL;


	return RegisterClassEx(&MainPW);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT R;
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������
   
   hWnd = CreateWindow(szWindowClass, _T("WPaintLight"), WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, MainBar() , hInstance, NULL);

   GetClientRect(hWnd, &R);
   ItemsBar = CreateWindow(_T("ItemsBar"), NULL, WS_CHILD, 0, 0, R.right - R.left, 80, hWnd, NULL, hInst, NULL);
  
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   ShowWindow(ItemsBar, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case CREATE:
			DrawArea = CreateWindow(_T("DrawAria"), NULL, WS_CHILD, 8, 85, 700, 500, hWnd, NULL, hInst, NULL);
			ShowWindow(DrawArea, SW_SHOW);
			break;
		case OPENF:
			break;
		case SAVEF:
			break;
		case SAVEFA:
			break;
		case PRINT:
			break;
		case ABOUT:
			break;
		case EX:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK	 DrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case EX:
			
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ItemsBarProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case EX:

			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HMENU MainBar()
{
	HMENU Bar = CreateMenu();
	HMENU File = CreatePopupMenu();
	AppendMenu(Bar, MF_STRING | MF_POPUP, (UINT)File, _T("����"));
	AppendMenu(File, MF_STRING, CREATE, _T("�������"));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, SAVEF, _T("�������..."));
	AppendMenu(File, MF_STRING, SAVEF, _T("���������"));
	AppendMenu(File, MF_STRING, SAVEF, _T("��������� ���...")); 
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, PRINT, _T("������..."));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, ABOUT, _T("� ���������..."));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, EX, _T("�����"));
	return Bar;
}