// testerToLab5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
typedef void __stdcall TLoadPhoneBase();
typedef void __stdcall TFindByName();

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (1)
	{
		std::cout << "*****Hello i'm tester for your 5 lab!!! Love me*****" << std::endl <<std::endl;
		HMODULE dataBaseLibrary = LoadLibrary(L"IndexDll.dll");
		if (dataBaseLibrary)
		{
			std::cout << "IndexDll.dll is successfully loaded" << std::endl;
		}
		else
		{
			std::cout << "Library isn't loaded" << std::endl;
			Sleep(1000);
			return -1;
		}
		TLoadPhoneBase* LoadPhoneBaseFunction = (TLoadPhoneBase*)GetProcAddress(dataBaseLibrary, "_LoadPhoneBase@0");
		if (LoadPhoneBaseFunction)
		{
			std::cout << "LoadPhoneBase is successfully loaded" << std::endl;
		}
		else
		{
			std::cout << "LoadPhoneBase isn't loaded" << std::endl;
			Sleep(1000);
			return -1;
		}
		TFindByName* FindByNameFunction = (TFindByName*)GetProcAddress(dataBaseLibrary, "_FindByName@26");
		LoadPhoneBaseFunction();
		char choiceString[2];

		char exitChoiceString[2];
		std::cout << "Press 'q' to quit." << std::endl;
		std::cin >> exitChoiceString;
		if (strcmp(exitChoiceString, "q") == 0)
			return 0;
	}
	
	return 0;
}

