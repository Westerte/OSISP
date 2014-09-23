// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Commctrl.h"
#include "Win32Project1.h"
#include "Commdlg.h"
#include "Wingdi.h"
#include "Math.h"

#define MAX_LOADSTRING 100
#define CREATE 1          //Create         Main menu comands
#define OPENF 2			  //Open file
#define SAVEF 3           //Save file
#define SAVEFA 4          //Save file ass
#define PRINT 5			  //Print
#define ABOUT 6           //About
#define EX 7             //Exit

#define TOOLBAR 8
#define PENCIL 9                    // Toolbar 
#define LINE 10            
#define RECTANGLE 11   
#define CIRCLE 12 
#define ELLIPSE 13
#define TRIANGEL 14
#define POLYGON 15
#define TEXT 16
#define EDITCONT 17
#define COMBOX 18
#define LABEL2 19
#define LABEL3 20
#define POLYLINE 21


// Глобальные переменные:
float k ;
HWND ItemsBar;
HWND DrawArea;
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
TCHAR szMainBar[MAX_LOADSTRING] = _T("MainBar");
HWND TextWnd;
LPENHMETAHEADER Header;
PRINTDLG pd;
HWND hWndComboBox, MainWind;
COLORREF Col = RGB(0, 0, 0);
COLORREF ColB = RGB(255, 255, 255);
int index;
HWND LineColor, BrushColor, ClockWind;
HDC hCompitableDC = NULL;


int CommandInd = 0;
int x, y;
int a, b;
int bx = -1, by = -1;
HBITMAP hbmp;
HDC hDC, ClockDC, dubmemDC;
HDC memDC = NULL;
int height, width;
RECT DrawAreaClientRect, DrawAreaRect ,R;
HGDIOBJ OldBrush;
HGDIOBJ OldPen;
TCHAR fileName[MAX_PATH] = _T("");
HWND label2;
HWND label3;
int x_k = 0, y_k = 0;
SYSTEMTIME st;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM            RegisterItemsBar(HINSTANCE hInstance);
ATOM            RegisterDrawingWind(HINSTANCE hInstance);
ATOM				MyRegisterClass(HINSTANCE hInstance);
ATOM RegisterColorsWindows(HINSTANCE hInstance);
ATOM RegisterClockWind(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void AllDraw(RECT R, WPARAM wParam, LPARAM lParam, int Kof1, int Kof2);

void PencilDrawing(int a, int b);
void DrawLine(int x, int y, int a, int b);
void DrawRectangle(int x, int y, int a, int b);
void DrawCircle(int x, int y, int a, int b);
void DrawEllipse(int x, int y, int a, int b);
void DrawTriangle(int x, int y, int a, int b);
void DrawTextT(int x, int y, int a, int b);
void SaveSomeFile(HDC dc, int width, int height);

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DrawProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ItemsBarProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ColorProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ClockProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HMENU MainBar();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	RegisterItemsBar(hInstance);          //Items Bar inicialization
  	RegisterDrawingWind(hInstance);       //Drawing area inicialization
	MyRegisterClass(hInstance);			//Main Window inicialization
	RegisterColorsWindows(hInstance);
	RegisterClockWind(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	k = 1.0;
	
	ACCEL acc[5];
	acc[0].cmd = CREATE;
	acc[0].fVirt = FVIRTKEY | FCONTROL;
	acc[0].key = 0x4E;
	acc[1].cmd = EX;
	acc[1].fVirt = FVIRTKEY | FCONTROL;
	acc[1].key = 0x51;
	acc[2].cmd = OPENF;
	acc[2].fVirt = FVIRTKEY | FCONTROL;
	acc[2].key = 0x4F;
	acc[3].cmd = PRINT;
	acc[3].fVirt = FVIRTKEY | FCONTROL;
	acc[3].key = 0x50;
	acc[4].cmd = SAVEF;
	acc[4].fVirt = FVIRTKEY | FCONTROL;
	acc[4].key = 0x53;
	hAccelTable = CreateAcceleratorTable((LPACCEL)acc, 5);

	// Цикл основного сообщения:
	
	

	
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(MainWind, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//

//Drawing Area Inicialization
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

ATOM RegisterClockWind(HINSTANCE hInstance)
{
	WNDCLASSEX DrawArea;

	DrawArea.cbSize = sizeof(WNDCLASSEX);
	DrawArea.style = CS_HREDRAW | CS_VREDRAW;
	DrawArea.lpfnWndProc = ClockProc;
	DrawArea.cbClsExtra = 0;
	DrawArea.cbWndExtra = 0;
	DrawArea.hInstance = hInstance;
	DrawArea.hIcon = NULL;
	DrawArea.hCursor = LoadCursor(NULL, IDC_ARROW);
	DrawArea.hbrBackground = (HBRUSH)(6);
	DrawArea.lpszMenuName = NULL;
	DrawArea.lpszClassName = _T("ClockWind");
	DrawArea.hIconSm = NULL;

	return RegisterClassEx(&DrawArea);
}
//Items Bar inicialization
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

ATOM RegisterColorsWindows(HINSTANCE hInstance)
{
	WNDCLASSEX ItemsBar;

	ItemsBar.cbSize = sizeof(WNDCLASSEX);
	ItemsBar.style = CS_HREDRAW | CS_VREDRAW;
	ItemsBar.lpfnWndProc = ColorProc;
	ItemsBar.cbClsExtra = 0;
	ItemsBar.cbWndExtra = 0;
	ItemsBar.hInstance = hInstance;
	ItemsBar.hIcon = NULL;
	ItemsBar.hCursor = LoadCursor(NULL, IDC_ARROW);
	ItemsBar.hbrBackground = CreateSolidBrush(Col);
	ItemsBar.lpszMenuName = NULL;
	ItemsBar.lpszClassName = _T("ColorClass");
	ItemsBar.hIconSm = NULL;

	return RegisterClassEx(&ItemsBar);
}


//Main Window inicialization
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
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   RECT R;
 
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   
   MainWind = hWnd = CreateWindow(szWindowClass, _T("WPaintLight"), WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, MainBar() , hInstance, NULL);

   GetClientRect(hWnd, &R);                                                                                      //Function to getting window size                                                            
   ItemsBar = CreateWindow(_T("ItemsBar"), NULL, WS_CHILD, 0, 0, R.right - R.left, 80, hWnd, NULL, hInst, NULL); //Items Bar Creating
  
   HWND PencilBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT ,
	   5, 5, 33, 33, ItemsBar, (HMENU)PENCIL, hInstance, NULL);
   HBITMAP PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PENCIL));
   SendMessage(PencilBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);
   
   HWND LineBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   39, 5, 33, 33, ItemsBar, (HMENU)LINE, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LINE));
   SendMessage(LineBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);
   
   HWND RectangleBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   73, 5, 33, 33, ItemsBar, (HMENU)RECTANGLE, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RECTANGLE));
   SendMessage(RectangleBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND CircleBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   107, 5, 33, 33, ItemsBar, (HMENU)CIRCLE, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CIRCLE));
   SendMessage(CircleBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND EllipseBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   5, 39, 33, 33, ItemsBar, (HMENU)ELLIPSE, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ELLIPSE));
   SendMessage(EllipseBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND TriangelBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   39, 39, 33, 33, ItemsBar, (HMENU)TRIANGEL, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRIANGEL));
   SendMessage(TriangelBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND PolygonBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   73, 39, 33, 33, ItemsBar, (HMENU)POLYGON, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POLYGON));
   SendMessage(PolygonBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND TextBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   107, 39, 33, 33, ItemsBar, (HMENU)TEXT, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TEXT));
   SendMessage(TextBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND PolyBut = CreateWindow(_T("BUTTON"), NULL, WS_CHILD | BS_BITMAP | BS_FLAT,
	   141, 5, 33, 33, ItemsBar, (HMENU)POLYLINE, hInstance, NULL);
   PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POLYLINE));
   SendMessage(PolyBut, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)PI);

   HWND label = CreateWindow(_T("static"), _T("Ширина линии:"),
	   WS_CHILD | WS_VISIBLE | WS_TABSTOP,
	   195, 5, 120, 23,
	   hWnd, NULL,
	   hInstance, NULL);
   hWndComboBox = CreateWindow(WC_COMBOBOX,NULL,
	   CBS_DROPDOWN | WS_CHILD | WS_VISIBLE | CBS_SIMPLE,
	   195, 30, 120, 180, ItemsBar, (HMENU)COMBOX, hInstance,
	   NULL);

   
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("1"));
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("2"));
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("3"));
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("4"));
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("5"));
   SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("6"));
   SendMessage(hWndComboBox, CB_SELECTSTRING, 0, (LPARAM)_T("1"));
   
   label2 = CreateWindow(_T("BUTTON"), _T("Цвет линии:"), WS_CHILD | BS_FLAT,
	   340, 5, 120, 23,
	   ItemsBar, (HMENU)LABEL2,
	   hInstance, NULL);
   LineColor = CreateWindow(_T("ColorClass"), NULL, WS_CHILD | WS_BORDER,
	   470, 5, 23, 23,
	   ItemsBar, NULL,
	   hInstance, NULL);
   BrushColor = CreateWindow(_T("ColorClass"), NULL, WS_CHILD|WS_BORDER,
	   470, 39, 23, 23,
	   ItemsBar, NULL,
	   hInstance, NULL);
   label3 = CreateWindow(_T("BUTTON"), _T("Цвет кисти:"), WS_CHILD | BS_FLAT,
	   340, 39, 120, 23,
	   ItemsBar, (HMENU)LABEL3,
	   hInstance, NULL);
  
   
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   ShowWindow(ItemsBar, nCmdShow);    
   UpdateWindow(hWnd);
   ShowWindow(PencilBut, nCmdShow);
   ShowWindow(LineBut, nCmdShow);
   ShowWindow(RectangleBut, nCmdShow);
   ShowWindow(CircleBut, nCmdShow);
   ShowWindow(EllipseBut, nCmdShow);
   ShowWindow(TriangelBut, nCmdShow);
   ShowWindow(PolygonBut, nCmdShow);
   ShowWindow(TextBut, nCmdShow);
   ShowWindow(PolyBut, nCmdShow);
   ShowWindow(label2, nCmdShow);
   ShowWindow(label3, nCmdShow);
   ShowWindow(LineColor, nCmdShow);
   ShowWindow(BrushColor, nCmdShow);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HGDIOBJ hOldbmp;
	HBRUSH hBrush, newBrush;
	HPEN newPen;
	LOGBRUSH PreBrush;
	int delta;
	HDC hdc;
	RECT RCT;
	GetClientRect(hWnd, &RCT);
	int mx0 = 90, my0 = 4;

	switch (message)
	{

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case CREATE:     //Drawing area creating
			
			SetTimer(hWnd, 1, 200, 0);
			k = 1;
			Col = RGB(0, 0, 0);
			ColB = RGB(255, 255, 255);
			RECT RCT;
			GetClientRect(LineColor, &RCT);
			InvalidateRect(LineColor, &RCT, FALSE);
			InvalidateRect(BrushColor, &RCT, FALSE);
			if (memDC != NULL)
			{
				KillTimer(hWnd, 1);
				ReleaseDC(DrawArea, hDC);
				ReleaseDC(DrawArea, memDC);
				DestroyWindow(DrawArea);
				hDC = NULL;
				memDC = NULL;
				DrawArea = NULL;
				if (TextWnd != NULL)
				{
					DestroyWindow(TextWnd);
					TextWnd = NULL;
				}
				if (bx != -1)
				{
					bx = -1;
					by = -1;
				}
				fileName[0] = '\0';
			}
			
			DrawArea = CreateWindow(_T("DrawAria"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 8 , 85, 1000, 580, hWnd, NULL, hInst, NULL);
			ClockWind = CreateWindow(_T("ClockWind"), NULL, WS_CHILD|WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 820, 0, 180, 182, DrawArea, NULL, hInst, NULL);
			hDC = GetDC(DrawArea);
			memDC = CreateCompatibleDC(hDC);
			dubmemDC = CreateCompatibleDC(hDC);
			GetWindowRect(DrawArea, &DrawAreaRect);
			GetClientRect(DrawArea, &DrawAreaClientRect);
			height = DrawAreaClientRect.bottom - DrawAreaClientRect.top;
			width = DrawAreaClientRect.right - DrawAreaClientRect.left;
			hbmp = CreateCompatibleBitmap(hDC, width , height);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			DeleteObject(SelectObject(memDC, hbmp));
			hbmp = CreateCompatibleBitmap(memDC, width, height);
			DeleteObject(SelectObject(dubmemDC, hbmp));
			FillRect(memDC, &DrawAreaClientRect, hBrush);
			FillRect(dubmemDC, &DrawAreaClientRect, hBrush);
			newPen = CreatePen(PS_SOLID, SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0)+1
            , Col);
			PreBrush.lbStyle = BS_HOLLOW;
			newBrush = CreateBrushIndirect(&PreBrush);
			DeleteObject(SelectObject(hDC, newBrush));
			DeleteObject(SelectObject(memDC, newBrush));
			DeleteObject(SelectObject(memDC, newPen));
			DeleteObject(SelectObject(hDC, newPen));
			
			ShowWindow(DrawArea, SW_SHOW);
			ShowWindow(ClockWind, SW_SHOW);
			break;
		case OPENF:       //Processing open file
			if (memDC != NULL)
			{
				ReleaseDC(DrawArea, hDC);
				ReleaseDC(DrawArea, memDC);
				ReleaseDC(DrawArea, dubmemDC);
				DestroyWindow(DrawArea);
				DestroyWindow(ClockWind);
				hDC = NULL;
				memDC = NULL;
				DrawArea = NULL;
				if (TextWnd != NULL)
				{
					DestroyWindow(TextWnd);
					TextWnd = NULL;
				}
				if (bx != -1)
				{
					bx = -1;
					by = -1;
				}
				k = 1;
			}
			Col = RGB(0, 0, 0);
			ColB = RGB(255, 255, 255);
			RECT RCTS;
			GetClientRect(LineColor, &RCTS);
			InvalidateRect(LineColor, &RCTS, FALSE);
			InvalidateRect(BrushColor, &RCTS, FALSE);
			OPENFILENAME ofn;
			
			RECTL HeaderRect;
			*fileName = 0;
			HMETAFILE h;
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = sizeof(fileName);
			ofn.lpstrFilter = _T(".emf\0*.emf");
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = _T("ОТКРЫТЬ");
			ofn.lpstrInitialDir = _T("c:\\windows");
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn))
			{
				
				HENHMETAFILE h = GetEnhMetaFile(ofn.lpstrFile);
				int siz = GetEnhMetaFileHeader(h, 0, NULL);
				Header = (LPENHMETAHEADER)GlobalAlloc(0, siz);
				GetEnhMetaFileHeader(h, siz, Header);
				DrawArea = CreateWindow(_T("DrawAria"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 8, 85, Header->rclBounds.right + 1,
					Header->rclBounds.bottom + 1, hWnd, NULL, hInst, NULL);
				ClockWind = CreateWindow(_T("ClockWind"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 820, 0,
					180, 182, DrawArea, NULL, hInst, NULL);
				SetTimer(hWnd, 1, 200, 0);
				hDC = GetDC(DrawArea);
				memDC = CreateCompatibleDC(hDC);
				GetWindowRect(DrawArea, &DrawAreaRect);
				GetClientRect(DrawArea, &DrawAreaClientRect);
				height = DrawAreaClientRect.bottom;
				width = DrawAreaClientRect.right;
				hbmp = CreateCompatibleBitmap(hDC, width, height);
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				DeleteObject(SelectObject(memDC, hbmp));
				dubmemDC = CreateCompatibleDC(hDC);
				hbmp = CreateCompatibleBitmap(memDC, width, height);
				DeleteObject(SelectObject(dubmemDC, hbmp));
				FillRect(memDC, &DrawAreaClientRect, hBrush);
				FillRect(dubmemDC, &DrawAreaClientRect, hBrush);
				newPen = CreatePen(PS_SOLID, SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0)+1
					, Col);
				PreBrush.lbStyle = BS_HOLLOW;
				
				newBrush = CreateBrushIndirect(&PreBrush);
				OldBrush = SelectObject(hDC, newBrush);
				DeleteObject(SelectObject(memDC, newPen));
				DeleteObject(SelectObject(dubmemDC, newPen));
				OldPen = SelectObject(hDC, newPen);
				PlayEnhMetaFile(dubmemDC, h, &DrawAreaClientRect);
				BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
				BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
				DeleteEnhMetaFile(h);
				ShowWindow(DrawArea, SW_SHOW);
				ShowWindow(ClockWind, SW_SHOW);
			}
			else
			{
				ReleaseDC(DrawArea, hDC);
				ReleaseDC(DrawArea, memDC);
				DestroyWindow(DrawArea);
				DestroyWindow(ClockWind);
				KillTimer(hWnd, 1);
				hDC = NULL;
				memDC = NULL;
				DrawArea = NULL;
				ClockWind = NULL;

			}
			
			break;        
		case SAVEF:       //Processing save file
			if (memDC != NULL)
				if (fileName[0] == '\0')
				{
					SaveSomeFile(memDC, width, height);
				}
				else
				{
					HDC Meta = CreateEnhMetaFile(dubmemDC, fileName, NULL, NULL);
					BitBlt(Meta, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
					HENHMETAFILE h = CloseEnhMetaFile(Meta);
					DeleteEnhMetaFile(h);
				}
			break;
		case SAVEFA:     //Processing save file ass
			if (memDC != NULL)
			SaveSomeFile(dubmemDC, width, height);
			break;
		case PRINT:      //Processing print 
			if (memDC != NULL)
			{

				ZeroMemory(&pd, sizeof(pd));
				pd.lStructSize = sizeof(pd);
				pd.hwndOwner = hWnd;
				pd.hDevMode = NULL;
				pd.hDevNames = NULL;
				pd.Flags = PD_RETURNDC | PD_NOSELECTION | PD_PRINTTOFILE | PD_NOPAGENUMS;
				pd.nMinPage = 1;
				pd.nMaxPage = 1;
				pd.nCopies = 1;
				if (PrintDlg(&pd) == TRUE)
				{
					DOCINFO doc;
					GlobalUnlock(pd.hDevMode);
					DEVNAMES * pdn = (DEVNAMES *)GlobalLock(pd.hDevNames);
					ZeroMemory(&doc, sizeof(doc));
					doc.cbSize = sizeof(doc);
					doc.lpszDatatype = _T("RAW");
					doc.lpszOutput = (TCHAR *)pdn + pdn->wOutputOffset;
					int lol = StartDoc(pd.hDC, &doc);
					StartPage(pd.hDC);
					BitBlt(pd.hDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
					EndPage(pd.hDC);
					EndDoc(pd.hDC);
					DeleteDC(pd.hDC);
				}
			}
			break;
		case ABOUT:        //Processing about
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case EX:          //Processing exit
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		if (TextWnd == NULL)
		AllDraw(DrawAreaClientRect, wParam, lParam, 8, 85);
		break;
	case WM_LBUTTONUP:
		if ((CommandInd == TEXT) && (TextWnd == NULL))
		{
			if (x < a && y < b)
			if ((x >= DrawAreaClientRect.left) && (x <= DrawAreaClientRect.right)
				&& (y >= DrawAreaClientRect.top) && (y <= DrawAreaClientRect.bottom))
			{
				TextWnd = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE
					| WS_BORDER | ES_MULTILINE, x, y, abs(x - a),
					abs(y - b), DrawArea, (HMENU)EDITCONT, hInst, NULL);
				ShowWindow(TextWnd, SW_SHOW);
			}
		}
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON) && (CommandInd != POLYLINE))
			BitBlt(dubmemDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		break;
	case WM_MOUSEWHEEL:
	if ((TextWnd == NULL) && (bx == -1))
		{
			delta = HIWORD(wParam);
			newBrush = CreateSolidBrush(RGB(255, 255, 255));
			if (GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
			{
				if (delta == 120)
				{
					x_k = x_k + 30;
					if (x_k >= 700)
					{
						x_k = 700;
						break;
					}
					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
				else
				{
					x_k = x_k - 30;
					if (x_k <= -700)
					{
						x_k = -700;
						break;
					}
					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
				break;
			}
			if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
			{
				if (delta == 120)
				{
					y_k = y_k + 30;
					if (y_k >= 400)
					{
						y_k = 400;
						break;
					}
					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
				else
				{
					y_k = y_k - 30;
					if (y_k <= -700)
					{
						y_k = -700;
						break;
					}
					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
				break;
			}
				if (delta == 120)
				{
					k -= 0.15;
					if (k <= 0.1)
					{
						k = 0.15;
						break;
					}


					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
				else
				{
					k += 0.15;
					if (k >= 4)
					{
						k = 3.85;
						break;
					}
					FillRect(hDC, &DrawAreaClientRect, newBrush);
					StretchBlt(hDC, 0, 0, width, height, memDC, 0 + x_k, 0 + y_k, (int)width*k, (int)height*k, SRCCOPY);
				}
			}
	break;
	case WM_TIMER:
		if (memDC != NULL)
		{
			GetClientRect(ClockWind, &RCT);
			InvalidateRect(ClockWind, &RCT, FALSE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0; 
}


//
//Draw Aria processing 
LRESULT CALLBACK	 DrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR buf[1024] = _T("");
	

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
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
		BitBlt(hdc, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		if (TextWnd == NULL)
		AllDraw(DrawAreaClientRect, wParam, lParam, 0 ,0);
		break;
	case WM_LBUTTONUP:
		if ((CommandInd == TEXT) && (TextWnd == NULL))
		{	
			if (x < a && y < b)
			if ((x >= DrawAreaClientRect.left) && (x <= DrawAreaClientRect.right)
				&& (y >= DrawAreaClientRect.top) && (y <= DrawAreaClientRect.bottom))
			{	TextWnd = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE
					| WS_BORDER | ES_MULTILINE, x, y, abs(x - a),
					abs(y - b), DrawArea, (HMENU)EDITCONT, hInst, NULL);
				ShowWindow(TextWnd, SW_SHOW);
			}
		}
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON))
			BitBlt(dubmemDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		break;

	case WM_RBUTTONDOWN:
		if (TextWnd != NULL)
		{
			SendMessage(TextWnd, EM_FMTLINES, TRUE, NULL);
			GetWindowText(TextWnd, buf, 1024);
			int i = 0;
			while (buf[i] != '\0')
			{
				if ((buf[i] == '\r') && (buf[i + 1] == '\r'))
				{
					int j = i;
					while (buf[j] != '\0')
					{
						buf[j] = buf[j+1];
						j++;
					}
				}
				i++;
			}
			SetRect(&R, x,y,a,b);
			SetBkMode(dubmemDC, TRANSPARENT);
			DrawText(dubmemDC, buf, 1024, &R, DT_LEFT | DT_TOP);
			DestroyWindow(TextWnd);
			BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
			BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
			TextWnd = NULL;
			UpdateWindow(DrawArea);
		}
			if (CommandInd == POLYLINE)
			{
				if (bx != -1)
				{
					MoveToEx(dubmemDC, x, y, NULL);
					LineTo(dubmemDC, a, b);
					bx = by = -1;
				}
			}
		
		break;
	case WM_LBUTTONDOWN:
		if (CommandInd == POLYGON)
		{
			if (bx == -1)
			{
				bx = x = LOWORD(lParam);
				by = y = HIWORD(lParam);
				
			}
			else
			{
				if ((abs(bx - a) <= 8) && (abs(by - b) <= 8))
				{
					MoveToEx(dubmemDC, x, y, NULL);
					LineTo(dubmemDC, bx, by);
					BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
					BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
					bx = by = -1;
				}
				else
				{
					a = LOWORD(lParam);
					b = HIWORD(lParam);
					MoveToEx(dubmemDC, x, y, NULL);
					LineTo(dubmemDC, a, b);
					x = a;
					y = b;
				}
			}
			
		}
		if (CommandInd == POLYLINE)
		{
			if (bx == -1)
			{
				bx = x = LOWORD(lParam);
				by = y = HIWORD(lParam);

			}
			else
			{
					a = LOWORD(lParam);
					b = HIWORD(lParam);
					MoveToEx(dubmemDC, x, y, NULL);
					LineTo(dubmemDC, a, b);
					x = a;
					y = b;
			}

		}
		k = 1;
		x_k = 0;
		y_k = 0;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		
	}
	return 0;
}

//
//Items Bar processing
LRESULT CALLBACK ItemsBarProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN newPen;
	CHOOSECOLOR COLOR;
	
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		bx = -1, by = -1;
		// Разобрать выбор в меню:
		if (TextWnd == NULL)
		switch (wmId)
		{
		case PENCIL:
			CommandInd = PENCIL;
			break;
		case LINE:
			CommandInd = LINE;
			break;
		case RECTANGLE:
			CommandInd = RECTANGLE;
			break;
		case CIRCLE:
			CommandInd = CIRCLE;
			break;
		case ELLIPSE:
			CommandInd = ELLIPSE;
			break;
		case TRIANGEL:
			CommandInd = TRIANGEL;
			break;
		case POLYGON:
			CommandInd = POLYGON;
			break;
		case TEXT:
			CommandInd = TEXT;
			break;
		case POLYLINE:
			CommandInd = POLYLINE;
			break;
		case COMBOX:
			index = SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0);
			if (memDC != 0)
			{
				newPen = CreatePen(PS_SOLID, index + 1, Col);
				DeleteObject(SelectObject(dubmemDC, newPen));
				DeleteObject(SelectObject(memDC, newPen));
				DeleteObject(SelectObject(hDC, newPen));
			}
			break;
		case LABEL2:
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hWnd;
			COLOR.lpCustColors = &Col;
			COLOR.Flags = CC_ANYCOLOR | CC_FULLOPEN;
			if ((ChooseColor(&COLOR) == TRUE) && (memDC != NULL))
			{
				Col = COLOR.rgbResult;
				HDC COL1 = GetDC(LineColor);
				HBRUSH newBrush = CreateSolidBrush(Col);
				RECT RCT;
				GetClientRect(LineColor, &RCT);
				FillRect(COL1, &RCT, newBrush);
				newPen = CreatePen(PS_SOLID, index + 1, Col);
				DeleteObject(SelectObject(dubmemDC, newPen));
				DeleteObject(SelectObject(memDC, newPen));
				DeleteObject(SelectObject(hDC, newPen));
			}
			break;
		case LABEL3:
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hWnd;
			COLOR.lpCustColors = &ColB;
			COLOR.Flags = CC_ANYCOLOR | CC_FULLOPEN;
			if ((ChooseColor(&COLOR) == TRUE) && (memDC != NULL))
			{
				ColB = COLOR.rgbResult;
				HDC COL2 = GetDC(BrushColor);
				HBRUSH newBrush = CreateSolidBrush(ColB);
				RECT RCT;
				GetClientRect(LineColor, &RCT);
				FillRect(COL2, &RCT, newBrush);
				DeleteObject(SelectObject(dubmemDC, newBrush));
				DeleteObject(SelectObject(memDC, newBrush));
				DeleteObject(SelectObject(hDC, newBrush));
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		if ((TextWnd == NULL))
			AllDraw(DrawAreaClientRect, wParam, lParam, 8, 85);
		break;
	case WM_LBUTTONUP:
		if ((CommandInd == TEXT) && (TextWnd == NULL))
		{	
			if (x < a && y < b)
			if ((x >= DrawAreaClientRect.left) && (x <= DrawAreaClientRect.right)
				&& (y >= DrawAreaClientRect.top) && (y <= DrawAreaClientRect.bottom))
			{
				TextWnd = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE
					| WS_BORDER | ES_MULTILINE, x, y, abs(x - a),
					abs(y - b), DrawArea, (HMENU)EDITCONT, hInst, NULL);
				ShowWindow(TextWnd, SW_SHOW);
			}
		}
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON) && (CommandInd != POLYLINE))
			BitBlt(dubmemDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		
	}
	return 0;
}


//main menu with only one field "File"
HMENU MainBar()
{
	HMENU Bar = CreateMenu();
	HMENU File = CreatePopupMenu();
	AppendMenu(Bar, MF_STRING | MF_POPUP, (UINT)File, _T("Файл"));      // Field "File"
	AppendMenu(File, MF_STRING, CREATE, _T("Создать     (Ctrl+N)"));                 //content of "File"  
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, OPENF, _T("Открыть...     (Ctrl+O)"));
	AppendMenu(File, MF_STRING, SAVEF, _T("Сохранить"));
	AppendMenu(File, MF_STRING, SAVEFA, _T("Сохранить как...     (Ctrl+S)"));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, PRINT, _T("Печать...     (Ctrl+P)"));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, ABOUT, _T("О Программе..."));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, EX, _T("Выйти     (Ctrl + Q)"));                      //content of "File"
	return Bar;
}

void AllDraw(RECT R, WPARAM wParam, LPARAM lParam , int Kof1, int Kof2 )
{
	
	
	if ((wParam != MK_LBUTTON))
	{
		if ((CommandInd != POLYGON)&&(CommandInd != POLYLINE))
		{
			x = LOWORD(lParam) - Kof1;
			y = HIWORD(lParam) - Kof2;
		}
		else
		{
			if (bx != -1)
			{
				a = LOWORD(lParam) - Kof1;
				b = HIWORD(lParam) - Kof2;
				if ((x >= R.left) && (x <= R.right)
					&& (y >= R.top) && (y <= R.bottom))
					DrawLine(x, y, a, b);
			}
		}
	}
	if ((wParam == MK_LBUTTON) && (CommandInd != POLYGON))
	{
		a = LOWORD(lParam) - Kof1;
		b = HIWORD(lParam) - Kof2;
		if (CommandInd == PENCIL)
			PencilDrawing(a, b);

		if (CommandInd == LINE)
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawLine(x, y, a, b);

		if (CommandInd == RECTANGLE)
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawRectangle(x, y, a, b);
		
		if (CommandInd == CIRCLE)
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawCircle(x, y, a, b);

		if (CommandInd == ELLIPSE)
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawEllipse(x, y, a, b);

		if (CommandInd == TRIANGEL)
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawTriangle(x, y, a, b);
		
		if (CommandInd == TEXT)
		{	
			if ((x >= R.left) && (x <= R.right)
				&& (y >= R.top) && (y <= R.bottom))
				DrawTextT(x, y, a, b);
			
		}
	}
}

void PencilDrawing(int a, int b)
{

	MoveToEx(memDC, x, y, NULL);
	LineTo(memDC, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	x = a;
	y = b;
}

void DrawLine(int x, int y, int a, int b)
{
	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	MoveToEx(memDC, x, y, NULL);
	LineTo(memDC, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

void DrawRectangle(int x, int y, int a, int b)
{
	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	Rectangle(memDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

void DrawCircle(int x, int y, int a, int b)
{

	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	if ((a > x && b > y) || (a < x && b < y))
		Ellipse(memDC, x, y, x + (b - y), b);
	else
		Ellipse(hDC, x, y, a, y - (a - x));
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

void DrawEllipse(int x, int y, int a, int b)
{
	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	Ellipse(memDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

void DrawTriangle(int x, int y, int a, int b)
{
	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	if (b > y)
	{
		MoveToEx(memDC, x, b, NULL);
		LineTo(memDC, a, b);
		LineTo(memDC, x + (a - x) / 2, y);
		LineTo(memDC, x, b);
	}
	else
	{
		MoveToEx(memDC, a, y, NULL);
		LineTo(memDC, x, y);
		LineTo(memDC, a + (x - a) / 2, b);
		LineTo(memDC, a, y);
	}
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

void DrawTextT(int x, int y, int a, int b)
{
	HGDIOBJ OldPen;
	HPEN NewPen = CreatePen(PS_DASH, 1, RGB(0,0,0));
	OldPen = SelectObject(memDC, (HPEN)NewPen);
	BitBlt(memDC, 0, 0, width, height, dubmemDC, 0, 0, SRCCOPY);
	if (x < a && y < b)
	Rectangle(memDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hDC, (HPEN)OldPen));
}

void SaveSomeFile(HDC dc, int width, int height)
{
	OPENFILENAME ofn;
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = _T(".emf\0*.emf");
	ofn.nFilterIndex = 0;//
	ofn.lpstrTitle = _T("СОХРАНИТЬ КАК");
	ofn.lpstrInitialDir = _T("c:\\windows");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	HDC Meta;
	if (GetSaveFileName(&ofn))
	{
		int i = 0;
		while (fileName[i] != '\0')
			i++;
		if ((fileName[i - 1] != 'f') && (fileName[i - 2] != 'm') &&
			(fileName[i - 3] != 'e') && (fileName[i - 4] != '.'))
		{

			fileName[i] = '.';
			fileName[i + 1] = 'e';
			fileName[i + 2] = 'm';
			fileName[i + 3] = 'f';
			fileName[i + 4] = '\0';
		}
		Meta = CreateEnhMetaFile(dc, fileName, NULL, NULL);
		BitBlt(Meta, 0, 0, width, height, dc, 0, 0, SRCCOPY);
		HENHMETAFILE h = CloseEnhMetaFile(Meta);
		DeleteEnhMetaFile(h);
	}

	return;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK ColorProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH newBrush;
	switch (message)
	{
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hWnd == LineColor)
			newBrush = CreateSolidBrush(Col);
		else
			newBrush = CreateSolidBrush(ColB);
			DeleteObject(SelectObject(hdc, newBrush));
			RECT RCT;
			GetClientRect(hWnd, &RCT);
			FillRect(hdc, &RCT, newBrush);
			EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


LRESULT CALLBACK ClockProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	RECT RCT;
	GetClientRect(hWnd, &RCT);
	int mx0 = 90, my0 = 4;
	HANDLE hbmp;
	HBRUSH hBrush;
	HPEN newPen;
	HBITMAP hBitmap;
	HBITMAP PI;
	
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
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
		
		PI = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CLOCK));
		hCompitableDC = CreateCompatibleDC(hdc);
		DeleteObject(SelectObject(hCompitableDC, PI));
		GetLocalTime(&st);
		newPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		DeleteObject(SelectObject(hdc, newPen));
		BitBlt(hdc, 0, 0, RCT.right, RCT.bottom, hCompitableDC, 0, 0, SRCCOPY);
		
		
		MoveToEx(hdc, 89, 91, NULL);
		LineTo(hdc, 89 + 88 * sin(3.14F / 30 * st.wSecond), 91 - 88 * cos(3.14F / 30 * st.wSecond));

		newPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		DeleteObject(SelectObject(hdc, newPen));

		MoveToEx(hdc, 89, 91, NULL);
		LineTo(hdc, 89 + 83 * sin(3.14F / 30 * st.wMinute), 91 - 83 * cos(3.14F / 30 * st.wMinute));

		newPen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
		DeleteObject(SelectObject(hdc, newPen));

		MoveToEx(hdc, 88, 91, NULL);
		LineTo(hdc, 88 + 65 * sin(3.14F / 6 * st.wHour), 88 - 65 * cos(3.14F / 6 * st.wHour));
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		if ((TextWnd == NULL))
			AllDraw(DrawAreaClientRect, wParam, lParam, -820, 0);
		break;
	case WM_LBUTTONUP:
		if ((CommandInd == TEXT) && (TextWnd == NULL))
		{
			if (x < a && y < b)
			if ((x >= DrawAreaClientRect.left) && (x <= DrawAreaClientRect.right)
				&& (y >= DrawAreaClientRect.top) && (y <= DrawAreaClientRect.bottom))
			{
				TextWnd = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE
					| WS_BORDER | ES_MULTILINE, x, y, abs(x - a),
					abs(y - b), DrawArea, (HMENU)EDITCONT, hInst, NULL);
				ShowWindow(TextWnd, SW_SHOW);
			}
		}
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON))
			BitBlt(dubmemDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}
