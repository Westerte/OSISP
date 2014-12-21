// IndexDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <iostream>
typedef struct Abonent
{
	char secondName[26];
	char number[15];
	char adress[87];
};

HANDLE fileMappingHandle;
__int64 numberOfBitsToFileView;
const int numberOfAbonentsIn1KB = 8;
__int64 numberOfAbonents = 0;

extern "C"__declspec(dllexport) void __stdcall LoadPhoneBase()
{
	HANDLE phisicFileHandle = CreateFile(L"FM.dat", GENERIC_READ | GENERIC_WRITE, 
										FILE_SHARE_READ | FILE_SHARE_WRITE, 
										NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	fileMappingHandle = CreateFileMapping(phisicFileHandle, NULL, PAGE_READWRITE, 1, 500000, NULL);
	CloseHandle(phisicFileHandle);
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	numberOfBitsToFileView = systemInfo.dwAllocationGranularity;
	Abonent** abonentsArray = (Abonent**)MapViewOfFile(fileMappingHandle, FILE_MAP_WRITE, 0, 0, numberOfBitsToFileView);
	FILE *phoneBaseFile;
	fopen_s(&phoneBaseFile, "PhoneDataBase.txt", "rt");
	char currentAbonent[128];
	char secondName[26];
	char number[15];
	char adress[87];
	int numberOfPages = 1;
	int hightOffset = 0;
	__int64 abonentsCounter = 0;
		while (fgets(currentAbonent, 128, phoneBaseFile) != 0)
	{
		int i = 0;

		while (currentAbonent[i] != '#')
		{
			secondName[i] = currentAbonent[i];
			i++;
		}
		secondName[i] = '\0';
		i++;
		int j = 0;
		while (currentAbonent[i] != '#')
		{
			adress[j] = currentAbonent[i];
			i++;
			j++;
		}
		adress[j] = '\0';
		i++;
		j = 0;
		while (currentAbonent[i] != '\n')
		{
			number[j] = currentAbonent[i];
			j++;
			i++;
		}
		number[j] = '\0';
		Abonent *newAbonent = new Abonent;
		strcpy_s(newAbonent->secondName, secondName);
		strcpy_s(newAbonent->number, number);
		strcpy_s(newAbonent->adress, adress);
		int sizeOfAbonentStruct = sizeof(*newAbonent);
		abonentsArray[abonentsCounter] = newAbonent;
		abonentsCounter++;
		if (abonentsCounter == numberOfBitsToFileView / 1024 * numberOfAbonentsIn1KB)
		{
			UnmapViewOfFile(abonentsArray);
			abonentsArray = (Abonent**)MapViewOfFile(fileMappingHandle, FILE_MAP_WRITE, hightOffset, numberOfPages*numberOfBitsToFileView
				, numberOfBitsToFileView);
			numberOfPages++;
			abonentsCounter = 0;
			if (numberOfPages * numberOfAbonentsIn1KB >= 0xffffffff)
			{
				hightOffset++;
				numberOfPages = 0;
			}
		}
		numberOfAbonents++;
	}
}

extern "C" _declspec(dllexport) char*** _stdcall Find(char stringToFind[87], int mod)
{
	std::vector<Abonent*> abonentsVector;
	int hightOffset = 0;
	int numberOfPages = 1;
	Abonent** abonentsArray = (Abonent**)MapViewOfFile(fileMappingHandle, FILE_MAP_WRITE, 0, 0, numberOfBitsToFileView);
	int j = 0;
	
	for (int i = 0; i < numberOfAbonents; i++)
	{
		if (j == numberOfBitsToFileView / 1024 * numberOfAbonentsIn1KB)
		{
			UnmapViewOfFile(abonentsArray);
			abonentsArray = (Abonent**)MapViewOfFile(fileMappingHandle, FILE_MAP_WRITE, hightOffset, numberOfPages*numberOfBitsToFileView
				, numberOfBitsToFileView);
			numberOfPages++;
			j = 0;
			if (numberOfPages * numberOfAbonentsIn1KB >= 0xffffffff)
			{
				hightOffset++;
				numberOfPages = 0;
			}
		}
		switch (mod)
		{
		case 0:
			if (strstr(abonentsArray[j]->secondName, stringToFind))
			{
				abonentsVector.push_back(abonentsArray[j]);
			}
			break;
		case 1:
			if (strstr(abonentsArray[j]->adress, stringToFind))
			{
				abonentsVector.push_back(abonentsArray[j]);
			}
			break;
		case 2:
			if (strcmp(abonentsArray[j]->number, stringToFind) == 0)
			{
				abonentsVector.push_back(abonentsArray[j]);
			}
			break;
		}
		
		j++;
	}
	int vectorSize = abonentsVector.size();
	char*** exitArray = new char**[vectorSize];
	for (int i = 0; i < vectorSize; i++)
	{
		char** resultArray = new char*[3];
		resultArray[0] = abonentsVector[i]->secondName;
		resultArray[1] = abonentsVector[i]->adress;
		resultArray[2] = abonentsVector[i]->number;
		exitArray[i] = resultArray;
	}
	return exitArray;
}
