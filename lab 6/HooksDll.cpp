#pragma once

#include "stdafx.h"
#include "Dbghelp.h"
#include "windows.h"
#include "TlHelp32.h"

DWORD GetProcessID(wchar_t* processName);
HANDLE logFile;
CRITICAL_SECTION criticalSection;
void ReplaceAllOldFunctionsToNewFunctions();

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitializeCriticalSection(&criticalSection);
		logFile = CreateFileW(L"D:\\Работы\\ОСиСП\\Лабораторная6\\Hooks\\Debug\\loggerFrom6Lab.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
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
			WriteProcessMemory(GetCurrentProcess(), adressOfAdressToOldFunction, &adressOfNewFunction, sizeof(adressOfNewFunction), NULL);
			return;
		}
	}
}

HANDLE MyCreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	WriteToLogFile(L"CreateFileW", lpFileName);
	return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

inline HMODULE ModuleFromAddress(const void* pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
		? (HMODULE)mbi.AllocationBase : NULL);
}

void ReplaceAllOldFunctionsToNewFunctions()
{
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "CreateFileW");
	ReplaceStandartFunctionToHoockFunction("Kernel32.dll", OriginalFunction, (PROC)MyCreateFileW, ModuleFromAddress(&MyCreateFileW));
}


