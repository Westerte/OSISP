// PhoneDataBase.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "PhoneDataBase.h"
#include <vector>
#include <string>

#define MAX_LOADSTRING 100
#define RESULT_BUTTON 203
#define SURNAME_EDIT 204
#define ADRESS_EDIT 205
#define TELEPHONE_EDIT 206
#define RESULT_EDIT 207
#define SURNAME_CHECKBOX 208
#define ADRESS_CHECKBOX 209
#define TELEPHONE_CHECKBOX 210

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
HWND hSurnameLabel;
HWND hTelephoneLabel;
HWND hAdressLabel;
HWND hSurnameEdit;
HWND hTelephoneEdit;
HWND hAdressEdit;
HWND hResultEdit;
HWND hFindSurnameButton;
HWND hFindAdressButton;
HWND hFindTelephoneButton;
HWND hFindAllButton;
HWND hSurnameCheckBox;
HWND hAdressCheckBox;
HWND hTelephoneCheckBox;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


typedef void __stdcall TCreateIndex();
typedef void __stdcall TLoadPhoneBase();
typedef void __stdcall TInsert(char secondName[255]);
typedef int __stdcall TDelete(char secondName[255]);
typedef char*** __stdcall TDataBaseSearchBySecondName(char secondName[20]);
typedef char*** __stdcall TDataBaseSearchByAdress(char secondName[20]);
typedef char*** __stdcall TDataBaseSearchPhoneNumber(char phoneNumber[15]);

HMODULE dataBaseLibrary = LoadLibrary(L"IndexDll.dll");
TCreateIndex* CreateIndexFunction;
TDelete* DeleteFunction;
TLoadPhoneBase* LoadPhoneBaseFunction;
TDataBaseSearchBySecondName* DataBaseSearchBySecondNameFunction;
TDataBaseSearchByAdress* DataBaseSearchByAdressFunction;
TDataBaseSearchPhoneNumber* DataBaseSearchPhoneNumberFunction;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				AddText(HWND hEdit, char* text);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PHONEDATABASE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHONEDATABASE));


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PHONEDATABASE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PHONEDATABASE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 810, 700, NULL, NULL, hInstance, NULL);


	int x, w, y, h;
	y = 10; h = 20;
	x = 30; w = 90;

	hSurnameCheckBox = CreateWindow(L"button", L"hSurnameCheckBox", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		x - 20, y, 20, 20, hWnd, (HMENU)SURNAME_CHECKBOX, hInstance, NULL);

	hSurnameLabel = CreateWindow(L"static", L"hSurnameLabel", WS_CHILD | WS_VISIBLE, x, y, w, h, hWnd, NULL, hInstance, NULL);
	SetWindowText(hSurnameLabel, L"ФИО:");

	hSurnameEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"hSurnameEdit", WS_CHILD | WS_VISIBLE | ES_LEFT,
		x + 130, y, w + 50, h, hWnd, (HMENU)SURNAME_EDIT, hInstance, NULL);
	SetWindowText(hSurnameEdit, L"");
	y = 40;

	hAdressCheckBox = CreateWindow(L"button", L"hAdressCheckBox", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		x - 20, y, 20, 20, hWnd, (HMENU)ADRESS_CHECKBOX, hInstance, NULL);

	hAdressLabel = CreateWindow(L"static", L"hAdressLabel", WS_CHILD | WS_VISIBLE, x, y, w, h, hWnd, NULL, hInstance, NULL);
	SetWindowText(hAdressLabel, L"Адрес:");

	hAdressEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"hAdressEdit", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
		x + 130, y, w + 50, h, hWnd, (HMENU)ADRESS_EDIT, hInstance, NULL);
	SetWindowText(hAdressEdit, L"");
	y = 70;

	hTelephoneCheckBox = CreateWindow(L"button", L"hTelephoneCheckBox", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		x - 20, y, 20, 20, hWnd, (HMENU)TELEPHONE_CHECKBOX, hInstance, NULL);

	hTelephoneLabel = CreateWindow(L"static", L"hTelephoneLabel", WS_CHILD | WS_VISIBLE, x, y, w + 40, h, hWnd, NULL, hInstance, NULL);
	SetWindowText(hTelephoneLabel, L"Номер телефона:");

	hTelephoneEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"hTelephoneEdit", WS_CHILD | WS_VISIBLE | ES_LEFT,
		x + 130, y, w + 50, h, hWnd, (HMENU)TELEPHONE_EDIT, hInstance, NULL);
	SetWindowText(hTelephoneEdit, L"");

	y = 100;

	hFindAllButton = CreateWindow(L"button", L"hFindAllButton", WS_CHILD | WS_VISIBLE,
		x - 20, y, w + 200, h * 2, hWnd, (HMENU)RESULT_BUTTON, hInstance, NULL);
	SetWindowText(hFindAllButton, L"Найти");

	y = 130;

	hResultEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"hResultEdit", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		x + 300, y - 120, w * 5, h * 30, hWnd, (HMENU)TELEPHONE_EDIT, hInstance, NULL);
	SetWindowText(hResultEdit, L"");

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CreateIndexFunction = (TCreateIndex*)GetProcAddress(dataBaseLibrary, "_CreateIndex@0");
	DeleteFunction = (TDelete*)GetProcAddress(dataBaseLibrary, "_DataBaseDelete@4");
	LoadPhoneBaseFunction = (TLoadPhoneBase*)GetProcAddress(dataBaseLibrary, "_LoadPhoneBase@0");
	DataBaseSearchBySecondNameFunction = (TDataBaseSearchBySecondName*)GetProcAddress(dataBaseLibrary, "_DataBaseSearchBySecondName@4");
	DataBaseSearchByAdressFunction = (TDataBaseSearchByAdress*)GetProcAddress(dataBaseLibrary, "_DataBaseSearchByAdress@4");
	DataBaseSearchPhoneNumberFunction = (TDataBaseSearchPhoneNumber*)GetProcAddress(dataBaseLibrary, "_DataBaseSearchPhoneNumber@4");
	CreateIndexFunction();
	LoadPhoneBaseFunction();
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	std::vector<char*> resultsVector;
	char *result;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		char* input;
		char* secondName;
		char* adress;
		char* phoneNumber;
		char ***abonentsArray;
		
		HWND hwndSurname;
		LRESULT resSurname;
		HWND hwndAdress;
		LRESULT resAdress;
		HWND hwndPhone;
		LRESULT resPhone;
		
		int count;
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			FreeLibrary(dataBaseLibrary);
			DestroyWindow(hWnd);
			break;
		case RESULT_BUTTON:
			resSurname = SendMessage(hSurnameCheckBox, BM_GETCHECK, 0, 0);
			resAdress = SendMessage(hAdressCheckBox, BM_GETCHECK, 0, 0);
			resPhone = SendMessage(hTelephoneCheckBox, BM_GETCHECK, 0, 0);
			if (resSurname == BST_UNCHECKED && resAdress == BST_UNCHECKED && resPhone == BST_UNCHECKED)
				break;
			adress = (char*)malloc(sizeof(char)* 40);
			secondName = (char*)malloc(sizeof(char)* 40);
			phoneNumber = (char*)malloc(sizeof(char)* 40);
			GetWindowTextA(hSurnameEdit, secondName, 40);
			if (resSurname == BST_CHECKED)
			{
				if (strcmp(secondName, "") == 0)
				{
					MessageBox(hWnd, L"Enter data", NULL, MB_OK);
					break;
				}
				abonentsArray = DataBaseSearchBySecondNameFunction(secondName);
				if (abonentsArray == 0)
					break;
				count = _msize(abonentsArray) / sizeof(abonentsArray[0]);
				for (int i = 0; i < count; i++)
				{
					result = new char[255];
					strcpy_s(result, 2, "");
					for (int j = 0; j < 4; j++)
					{
						strcat_s(result, _msize(result) + 1, abonentsArray[i][j]);		
						strcat_s(result,_msize(result) + 1, " ");
					}
					resultsVector.push_back(result);
				}
			}
			GetWindowTextA(hAdressEdit, adress, 40);
			if (strcmp(adress, "") == 0 && resAdress == BST_CHECKED)
			{
				MessageBox(hWnd, L"Enter data", NULL, MB_OK);
				break;
			}
			if (resAdress == BST_CHECKED && resSurname == BST_UNCHECKED)
			{
				abonentsArray = DataBaseSearchByAdressFunction(adress);
				if (abonentsArray == 0)
					break;
				count = _msize(abonentsArray) / sizeof(abonentsArray[0]);
				for (int i = 0; i < count; i++)
				{
					result = new char[255];
					strcpy_s(result, 2, "");
					for (int j = 0; j < 4; j++)
					{
						strcat_s(result, _msize(result) + 1, abonentsArray[i][j]);
						strcat_s(result, _msize(result) + 1, " ");
					}
					resultsVector.push_back(result);
				}
			}

			GetWindowTextA(hTelephoneEdit, phoneNumber, 40);
			if (strcmp(phoneNumber, "") == 0 && resPhone == BST_CHECKED)
			{
				MessageBox(hWnd, L"Enter data", NULL, MB_OK);
				break;
			}
			if (resPhone == BST_CHECKED && resSurname == BST_UNCHECKED && resAdress == BST_UNCHECKED)
			{
				
				abonentsArray = DataBaseSearchPhoneNumberFunction(phoneNumber);
				if (abonentsArray == 0)
					break;
				count = _msize(abonentsArray) / sizeof(abonentsArray[0]);
				for (int i = 0; i < count; i++)
				{
					result = new char[255];
					strcpy_s(result, _msize(result) + 1, "");
					for (int j = 0; j < 4; j++)
					{
						strcat_s(result, _msize(result) + 1, abonentsArray[i][j]);
						strcat_s(result, _msize(result) + 1, " ");
					}
					resultsVector.push_back(result);
				}
			}
			SetWindowText(hResultEdit, L"");
			for (int i = 0; i < resultsVector.size(); i++)
			{
				if (strstr(resultsVector[i], secondName) != 0 || resSurname == BST_UNCHECKED)
				{
					if (strstr(resultsVector[i], adress) != 0 || resAdress == BST_UNCHECKED)
					{
						if (strstr(resultsVector[i], phoneNumber) != 0 || resPhone == BST_UNCHECKED)
						{
							AddText(hResultEdit, resultsVector[i]);
							AddText(hResultEdit, "\n");
						}
					}
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
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

void AddText(HWND hEdit, char* text)
{
	SendMessageA(hEdit, EM_SETSEL, -1, -1);
	SendMessageA(hEdit, EM_REPLACESEL, FALSE, (LPARAM)text);
}

// Message handler for about box.
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
