// testerTo6lab.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout << "*****Good day, man, i'm TESTER for your 6 lab, please be gentle with me*****" << std::endl;
	wchar_t exitChoice[2] = L"y";
	std::wcin >> exitChoice;
	HANDLE logFile = CreateFileW(L"D:\\loggerFrom6Lab1.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	std::wcin >> exitChoice;
	WriteFile(logFile, "лалала", 6, NULL, NULL);
	std::wcin >> exitChoice;
	char lol[7];
	ReadFile(logFile, lol, 6, NULL, NULL);
	std::wcin >> exitChoice;
	HKEY newRegistrKey = 0;
	HKEY result;
	RegCreateKeyExW(newRegistrKey, L"VITALUR", 0, NULL, REG_OPTION_NON_VOLATILE, NULL, NULL, &result, NULL);
	std::wcin >> exitChoice;
	RegOpenKeyExW(newRegistrKey, L"лалала", NULL, NULL, &result);
	std::wcin >> exitChoice;
	RegCloseKey(newRegistrKey);
	std::wcin >> exitChoice;

	return 0;
}

