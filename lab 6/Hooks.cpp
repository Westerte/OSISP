// Hooks.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "windows.h"
#include "TlHelp32.h"
#include "Dbghelp.h"

DWORD idProcessThatWillHaveMyDll;

DWORD GetProcessIDByProcessName(wchar_t* processName, std::vector<DWORD> &returnableVectorOfAllProccessIDWithThisName);
std::vector<int> addDllToAllProcessByIDs(std::vector<DWORD> allProccessID);
bool WriteMyDllPathToRemoteProcessAdressSpace(HANDLE remoteProcessHandle, wchar_t* pathOfDll, wchar_t* &baseAdressToInject);
bool CreateRemoteThreadWithMyDll(HANDLE remoteProcessHandle, void *baseAdressToInject);
BOOL SetPrivilege(HANDLE token, LPCTSTR privilegeName, BOOL isEnabled);

wchar_t libraryPath[55] = L"D:\\Работы\\ОСиСП\\Лабораторная6\\Hooks\\Debug\\HooksDll.dll";

int _tmain(int argc, _TCHAR* argv[])
{		
	std::wcout << "*****Good day, man, i'm your 6 lab, please be gentle with me*****" << std::endl;
	wchar_t exitChoice[2] = L"y";
	while (wcscmp(exitChoice, L"y") == 0)
	{
		wchar_t* enteredProcessName = new wchar_t[30];
		
		std::wcout << "Enter process name: ";
		std::wcin >> enteredProcessName;
		std::vector<DWORD> allProccessIDWithThisName;
		idProcessThatWillHaveMyDll = GetProcessIDByProcessName(enteredProcessName, allProccessIDWithThisName);
		int numberOfProcessWithCurrentName = allProccessIDWithThisName.size();
		if (numberOfProcessWithCurrentName == 0)
		{
			std::wcout << "Maybe we have a problem, maybe you have a problem)" << std::endl;
		}
		else
		{
			std::wcout << "PIDs with current name: " << std::endl;
			for (int i = 0; i < numberOfProcessWithCurrentName; i++)
			{
				std::wcout << allProccessIDWithThisName[i] << std::endl;
			}
			std::vector<int> truePidsNumbersInAllProcessID = addDllToAllProcessByIDs(allProccessIDWithThisName);
			int numberOfTruePids = truePidsNumbersInAllProcessID.size();
			if (numberOfTruePids == 0)
			{
				std::wcout << "Daaaamn, we can't do this processes" << std::endl;
			}
			else
			{
				std::wcout << "We have owned processes with PIDs:" << std::endl;
				for (int i = 0; i < numberOfTruePids; i++)
				{
					std::wcout << allProccessIDWithThisName[truePidsNumbersInAllProcessID[i]] << std::endl;
				}
			}
		}
		std::wcout << "Do you want to continue? y/n ";
		std::wcin >> exitChoice;
	}
	return 0;
}



DWORD GetProcessIDByProcessName(wchar_t* processName, std::vector<DWORD> &returnableVectorOfAllProccessIDWithThisName)
{
	std::vector<DWORD> allProcessIDWithThisName;
	HANDLE snapshotHandle;
	if ((snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		CloseHandle(snapshotHandle);
		return 0;
	}
	PROCESSENTRY32W processEntryToCurrentProcess;
	processEntryToCurrentProcess.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(snapshotHandle, &processEntryToCurrentProcess))
	{
		CloseHandle(snapshotHandle);
		return 0;
	}
	do
	{
		if (wcscmp(processEntryToCurrentProcess.szExeFile, processName) == 0)
		{
			allProcessIDWithThisName.push_back(processEntryToCurrentProcess.th32ProcessID);
		}
	} while (Process32NextW(snapshotHandle, &processEntryToCurrentProcess));
	CloseHandle(snapshotHandle);
	returnableVectorOfAllProccessIDWithThisName = allProcessIDWithThisName;
	return 1;
}

std::vector<int> addDllToAllProcessByIDs(std::vector<DWORD> allProccessID)
{
	std::vector<int> truePidsNumbersInALLProcessID;
	int vectorSize = allProccessID.size();
	int counterOfTrueHandles = 0;
	for (int i = 0; i < vectorSize; i++)
	{
		HANDLE currentProcessHandle = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, NULL, allProccessID[i]);
		if (currentProcessHandle != INVALID_HANDLE_VALUE)
		{
			wchar_t* baseAdressToInject = new wchar_t[55];
			if (WriteMyDllPathToRemoteProcessAdressSpace(currentProcessHandle, libraryPath, baseAdressToInject))
			{
				if (CreateRemoteThreadWithMyDll(currentProcessHandle, baseAdressToInject))
				{
					truePidsNumbersInALLProcessID.push_back(i);
					/*VirtualFreeEx(currentProcessHandle, baseAdressToInject, (lstrlenW(libraryPath) + 1)*sizeof(wchar_t), MEM_RELEASE);
					PTHREAD_START_ROUTINE freeLibraryThreadRoutine = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "FreeLibrary");
					HANDLE remoteThreadToFreeLibrary = CreateRemoteThread(currentProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)freeLibraryThreadRoutine, 0, 0, NULL);*/
					/*WaitForSingleObject(remoteThreadToFreeLibrary, INFINITE);*/
				}
			}
		}
	}
	return truePidsNumbersInALLProcessID;
}

bool WriteMyDllPathToRemoteProcessAdressSpace(HANDLE remoteProcessHandle, wchar_t* pathOfDll, wchar_t* &baseAdressToInject)
{
	baseAdressToInject = (wchar_t*)VirtualAllocEx(remoteProcessHandle, NULL, sizeof(pathOfDll), MEM_COMMIT, PAGE_READWRITE);
	DWORD writtenBytes;
	if (!WriteProcessMemory(remoteProcessHandle, baseAdressToInject, pathOfDll, (lstrlenW(pathOfDll) + 1)*sizeof(wchar_t), &writtenBytes))
	{
		return false;
	}
	return true;
}

bool CreateRemoteThreadWithMyDll(HANDLE remoteProcessHandle, void* baseAdressToInject)
{
	PTHREAD_START_ROUTINE loadLibraryThreadRoutine = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	HANDLE currentRemoteThread;
	if ((currentRemoteThread = CreateRemoteThread(remoteProcessHandle, NULL, 0, loadLibraryThreadRoutine, baseAdressToInject, 0, NULL)) == 0)
	{
		DWORD lastError = GetLastError();
		CloseHandle(currentRemoteThread);
		return false;
	}
	WaitForSingleObject(currentRemoteThread, INFINITE);
	CloseHandle(currentRemoteThread);
	return true;
}


