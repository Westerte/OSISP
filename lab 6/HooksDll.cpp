#pragma once

#include "stdafx.h"
#include "Dbghelp.h"
#include "windows.h"
#include "TlHelp32.h"
#include <iostream>

DWORD GetProcessID(wchar_t* processName);
HANDLE logFile;
CRITICAL_SECTION criticalSection;
void ReplaceAllOldFunctionsToNewFunctionsInAllProcessModules();

typedef HANDLE(WINAPI *CREATEFILEW)(LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitializeCriticalSection(&criticalSection);
		logFile = CreateFileW(L"D:\\loggerFrom6Lab.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
		ReplaceAllOldFunctionsToNewFunctionsInAllProcessModules();
		break;
	case DLL_PROCESS_DETACH:
		CloseHandle(logFile);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;

	}
	return TRUE;
}

void WriteToLogFile(LPCWSTR function, LPCWSTR parameter)
{
	EnterCriticalSection(&criticalSection);
	wchar_t messageToLog[300] = L"";
	wcscat_s(messageToLog, function);
	wcscat_s(messageToLog, L" ");
	wcscat_s(messageToLog, parameter);
	wcscat_s(messageToLog, L"\r\n");
	DWORD t = 0;
	WriteFile(logFile, messageToLog, (lstrlenW(messageToLog) * sizeof(wchar_t)), &t, NULL);
	LeaveCriticalSection(&criticalSection);
}

void ReplaceStandartFunctionToHoockFunction(LPCSTR moduleNameInThatWeReplaceFunction, PROC adressOfOldFunction,
													PROC adressOfNewFunction, HMODULE moduleThatContainNewFunction)
{
	ULONG sizeOfImport;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
		moduleThatContainNewFunction, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &sizeOfImport);
	if (importDescriptor == NULL)
		return;
	for (; importDescriptor->Name; importDescriptor++)
	{
		PSTR pszModName = (PSTR)((PBYTE)moduleThatContainNewFunction + importDescriptor->Name);
		if (lstrcmpiA(pszModName, moduleNameInThatWeReplaceFunction) == 0)
			break;
	}
	if (importDescriptor->Name == 0)
		return;
	PIMAGE_THUNK_DATA informationAboutStandartFunctionIdentificators =
		(PIMAGE_THUNK_DATA)((PBYTE)moduleThatContainNewFunction + importDescriptor->FirstThunk);
	for (; informationAboutStandartFunctionIdentificators->u1.Function; informationAboutStandartFunctionIdentificators++)
	{
		PROC* adressOfAdressToOldFunction = (PROC*)&informationAboutStandartFunctionIdentificators->u1.Function;
		if (*adressOfAdressToOldFunction == adressOfOldFunction)
		{
			if (!WriteProcessMemory(GetCurrentProcess(), adressOfAdressToOldFunction, &adressOfNewFunction, sizeof(adressOfNewFunction), NULL) && GetLastError() == ERROR_NOACCESS)
			{
				DWORD dwOldProtect;
				if (VirtualProtect(adressOfAdressToOldFunction, sizeof(adressOfNewFunction), PAGE_WRITECOPY,
					&dwOldProtect))
				{
					WriteProcessMemory(GetCurrentProcess(), adressOfAdressToOldFunction, &adressOfNewFunction,
						sizeof(adressOfNewFunction), NULL);
					VirtualProtect(adressOfAdressToOldFunction, sizeof(adressOfNewFunction), dwOldProtect,
						&dwOldProtect);
				}
			}
			return;
		}
	}
}

HANDLE WINAPI MyCreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	std::wcout << "it's strange" << std::endl;
	WriteToLogFile(L"CreateFileW", lpFileName);
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "CreateFileW");
	return ((CREATEFILEW)OriginalFunction)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

void ReplaceAllOldFunctionsToNewFunctionsInAllProcessModules()
{

	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "CreateFileW");
	HANDLE snapOfModules = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 currentModule;
	currentModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapOfModules, &currentModule))
	{
		do
		{			
			ReplaceStandartFunctionToHoockFunction("Kernel32.dll", OriginalFunction, (PROC)MyCreateFileW, currentModule.hModule);
		} 
		while (Module32Next(snapOfModules, &currentModule));
	}
}


