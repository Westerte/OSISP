// testerTo6lab.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	return 0;
}

