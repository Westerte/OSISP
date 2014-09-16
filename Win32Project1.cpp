// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Commctrl.h"
#include "Win32Project1.h"

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

// Глобальные переменные:
HWND ItemsBar;
HWND DrawArea;
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
TCHAR szMainBar[MAX_LOADSTRING] = _T("MainBar");
HWND TextWnd;


int CommandInd = 0;
int x, y;
int a, b;
int bx = -1, by = -1;
HBITMAP hbmp;
HDC hDC;
HDC memDC;
int height, width;
RECT DrawAreaClientRect, DrawAreaRect ,R;
HGDIOBJ OldBrush;
HGDIOBJ OldPen;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM            RegisterItemsBar(HINSTANCE hInstance);
ATOM            RegisterDrawingWind(HINSTANCE hInstance);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void AllDraw(RECT R, WPARAM wParam, LPARAM lParam, int Kof1, int Kof2);
void PencilDrawing(int a, int b);
void DrawLine(int x, int y, int a, int b);
void DrawRectangle(int x, int y, int a, int b);
void DrawCircle(int x, int y, int a, int b);
void DrawEllipse(int x, int y, int a, int b);
void DrawTriangle(int x, int y, int a, int b);
void DrawTextT(int x, int y, int a, int b);

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

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	RegisterItemsBar(hInstance);          //Items Bar inicialization
  	RegisterDrawingWind(hInstance);       //Drawing area inicialization
	MyRegisterClass(hInstance);			//Main Window inicialization

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// Цикл основного сообщения:
	
	

	
	
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
   
   hWnd = CreateWindow(szWindowClass, _T("WPaintLight"), WS_OVERLAPPEDWINDOW,
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
	HDC hdc;
	HGDIOBJ hOldbmp;
	HBRUSH hBrush, newBrush;
	HPEN newPen;
	LOGBRUSH PreBrush;
	
	

	switch (message)
	{

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case CREATE:     //Drawing area creating
			DrawArea = CreateWindow(_T("DrawAria"), NULL, WS_CHILD, 8 , 85, 700, 500, hWnd, NULL, hInst, NULL);
			hDC = GetDC(DrawArea);
			memDC = CreateCompatibleDC(hDC);
			GetWindowRect(DrawArea, &DrawAreaRect);
			GetClientRect(DrawArea, &DrawAreaClientRect);
			height = DrawAreaClientRect.bottom - DrawAreaClientRect.top;
			width = DrawAreaClientRect.right - DrawAreaClientRect.left;
			hbmp = CreateCompatibleBitmap(hDC, width , height);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			hOldbmp = SelectObject(memDC, hbmp);
			FillRect(memDC, &DrawAreaClientRect, hBrush);
			newPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			PreBrush.lbStyle = BS_HOLLOW;
			newBrush = CreateBrushIndirect(&PreBrush);
		    OldBrush = SelectObject(hDC, newBrush);
			DeleteObject(SelectObject(memDC, newPen));
			OldPen = SelectObject(hDC, newPen);
			ShowWindow(DrawArea, SW_SHOW);
			break;
		case OPENF:       //Processing open file
			break;        
		case SAVEF:       //Processing save file
			break;
		case SAVEFA:     //Processing save file ass
			break;
		case PRINT:      //Processing print 
			break;
		case ABOUT:        //Processing about
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
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON))
			BitBlt(memDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
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
		BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
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
			BitBlt(memDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
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
			SetBkMode(memDC, TRANSPARENT);
			DrawText(memDC, buf, 1024, &R, DT_LEFT | DT_TOP);
			DestroyWindow(TextWnd);
			BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
			TextWnd = NULL;
			UpdateWindow(DrawArea);
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
					MoveToEx(memDC, x, y, NULL);
					LineTo(memDC, bx, by);
					BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
					bx = by = -1;
				}
				else
				{
					a = LOWORD(lParam);
					b = HIWORD(lParam);
					MoveToEx(memDC, x, y, NULL);
					LineTo(memDC, a, b);
					x = a;
					y = b;
				}
			}
			
			
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
//Items Bar processing
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
		if ((CommandInd != TEXT) && (TextWnd == NULL) && (CommandInd != POLYGON))
			BitBlt(memDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
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
	AppendMenu(File, MF_STRING, CREATE, _T("Создать"));                 //content of "File"  
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, SAVEF, _T("Открыть..."));
	AppendMenu(File, MF_STRING, SAVEF, _T("Сохранить"));
	AppendMenu(File, MF_STRING, SAVEF, _T("Сохранить как...")); 
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, PRINT, _T("Печать..."));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, ABOUT, _T("О Программе..."));
	AppendMenu(File, MF_SEPARATOR, NULL, _T(""));
	AppendMenu(File, MF_STRING, EX, _T("Выйти"));                      //content of "File"
	return Bar;
}

void AllDraw(RECT R, WPARAM wParam, LPARAM lParam , int Kof1, int Kof2 )
{
	if ((wParam != MK_LBUTTON))
	{
		if (CommandInd != POLYGON)
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
	MoveToEx(hDC, x, y, NULL);
	LineTo(hDC, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	MoveToEx(hDC, x, y, NULL);
	LineTo(hDC, a, b);
}

void DrawRectangle(int x, int y, int a, int b)
{
	Rectangle(hDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	Rectangle(hDC, x, y, a, b);
}

void DrawCircle(int x, int y, int a, int b)
{
	if ((a > x && b > y) || (a < x && b < y))
		Ellipse(hDC, x, y, x + (b - y), b );
	else
		Ellipse(hDC, x, y, a, y - (a - x));
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	if ((a > x && b > y) || (a < x && b < y))
		Ellipse(hDC, x, y, x + (b - y), b);
	else
		Ellipse(hDC, x, y, a, y - (a - x));
}

void DrawEllipse(int x, int y, int a, int b)
{
	Ellipse(hDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	Ellipse(hDC, x, y, a, b);
}

void DrawTriangle(int x, int y, int a, int b)
{
	if (b > y)
	{	
		MoveToEx(hDC, x, b, NULL);
		LineTo(hDC, a, b);
		LineTo(hDC, x + (a - x) / 2, y);
		LineTo(hDC, x, b);
	}
	else
	{
		MoveToEx(hDC, a, y, NULL);
		LineTo(hDC, x, y);
		LineTo(hDC, a + (x - a) / 2, b);
		LineTo(hDC, a, y);
	}
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	if (b > y)
	{
		MoveToEx(hDC, x, b, NULL);
		LineTo(hDC, a, b);
		LineTo(hDC, x + (a - x) / 2, y);
		LineTo(hDC, x, b);
	}
	else
	{
		MoveToEx(hDC, a, y, NULL);
		LineTo(hDC, x, y);
		LineTo(hDC, a + (x - a) / 2, b);
		LineTo(hDC, a, y);
	}
}

void DrawTextT(int x, int y, int a, int b)
{
	HGDIOBJ OldPen;
	HPEN NewPen = CreatePen(PS_DASH, 1, RGB(0,0,0));
	OldPen = SelectObject(hDC, (HPEN)NewPen);
	if (x < a && y < b)
	Rectangle(hDC, x, y, a, b);
	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	if (x < a && y < b)
	Rectangle(hDC, x, y, a, b);
	DeleteObject(SelectObject(hDC, (HPEN)OldPen));
}
