// testerToLab5.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
typedef void __stdcall TLoadPhoneBase();
typedef char*** __stdcall TFindByName(char*, int);

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
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
		TFindByName* FindByNameFunction = (TFindByName*)GetProcAddress(dataBaseLibrary, "_Find@8");
		LoadPhoneBaseFunction();
		int choiseNumber;
		while (1)
		{
			std::cout << "1. Find by name" << std::endl;
			std::cout << "2. Find by adress" << std::endl;
			std::cout << "1. Find by number" << std::endl;
			std::cout << "Your choise: ";
			std::cin >> choiseNumber;
			char stringToFind[87];
			char*** abonentsArray = 0;
			switch (choiseNumber)
			{
			case 1:
				std::cout << "Enter name: ";
				std::cin >> stringToFind;
				abonentsArray = FindByNameFunction(stringToFind, 0);
				break;
			case 2:
				std::cout << "Enter adress: ";
				std::cin >> stringToFind;
				abonentsArray = FindByNameFunction(stringToFind, 1);
				break;
			case 3:
				std::cout << "Enter number: ";
				std::cin >> stringToFind;
				abonentsArray = FindByNameFunction(stringToFind, 2);
			}
			int count = _msize(abonentsArray) / sizeof(abonentsArray[0]);
			for (int i = 0; i < count; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					std::cout << abonentsArray[i][j];
					std::cout << " ";
				}
				std::cout << std::endl;
			}
			if (abonentsArray == 0)
			{
				continue;
			}
			std::cout << std::endl;
			char exitChoiseString[2];
			std::cout << "Press 'q' to quit." << std::endl;
			std::cin >> exitChoiseString;
			if (strcmp(exitChoiseString, "q") == 0)
				return 0;
		}
		return 0;
}

