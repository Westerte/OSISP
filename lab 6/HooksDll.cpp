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

typedef BOOL(WINAPI *WRITEFILE)(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped);

typedef BOOL(WINAPI *READFILE)(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped);

typedef LONG(WINAPI *REGCREATEKEYEXW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD Reserved,
	LPTSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition);

typedef LONG(WINAPI *REGOPENKEYEXW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD ulOptions,
	REGSAM samDesired,
	PHKEY phkResult);

typedef LONG(WINAPI* REGCLOSEKEY)(
	HKEY hKey);


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
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "WriteFile");
	((WRITEFILE)OriginalFunction)(logFile, messageToLog, (lstrlenW(messageToLog) * sizeof(wchar_t)), &t, NULL);
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
	WriteToLogFile(L"CreateFileW", lpFileName);
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "CreateFileW");
	return ((CREATEFILEW)OriginalFunction)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL WINAPI MyWriteFile(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped)
{
	wchar_t parametrsToLog[20];
	_ltow_s(nNumberOfBytesToWrite, parametrsToLog, 20);
	WriteToLogFile(L"WriteFile", L"LOL");
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "WriteFile");
	return ((WRITEFILE)OriginalFunction)(hFile,	lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL WINAPI MyReadFile(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	wchar_t parametrsToLog[20];
	_ltow_s(nNumberOfBytesToRead, parametrsToLog, 20);
	WriteToLogFile(L"ReadFile", parametrsToLog);
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "ReadFile");
	return ((WRITEFILE)OriginalFunction)(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

LONG WINAPI MyRegCreateKeyExW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD Reserved,
	LPTSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition)
{
	WriteToLogFile(L"RegCreateKeyExW", lpSubKey);
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegCreateKeyExW");
	return ((REGCREATEKEYEXW)OriginalFunction)(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}
LONG WINAPI MyRegOpenKeyExW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD ulOptions,
	REGSAM samDesired,
	PHKEY phkResult)
{
	WriteToLogFile(L"RegOpenKeyExW", lpSubKey);
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegOpenKeyExW");
	return ((REGOPENKEYEXW)OriginalFunction)(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LONG WINAPI MyRegCloseKey(
	HKEY hKey)
{
	WriteToLogFile(L"RegCloseKey", L"");
	PROC OriginalFunction = GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegCloseKey");
	return ((REGCLOSEKEY)OriginalFunction)(hKey);
}

void ReplaceAllOldFunctionsToNewFunctionsInAllProcessModules()
{
	HANDLE snapOfModules = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 currentModule;;
	currentModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapOfModules, &currentModule))
	{
		do
		{			
			ReplaceStandartFunctionToHoockFunction("Kernel32.dll", GetProcAddress(GetModuleHandleA("Kernel32.dll"), "CreateFileW"), (PROC)MyCreateFileW, currentModule.hModule);
			ReplaceStandartFunctionToHoockFunction("Kernel32.dll", GetProcAddress(GetModuleHandleA("Kernel32.dll"), "WriteFile"), (PROC)MyWriteFile, currentModule.hModule);	
			ReplaceStandartFunctionToHoockFunction("Kernel32.dll", GetProcAddress(GetModuleHandleA("Kernel32.dll"), "ReadFile"), (PROC)MyReadFile, currentModule.hModule);
			ReplaceStandartFunctionToHoockFunction("Advapi32.dll", GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegCreateKeyExW"), (PROC)MyRegCreateKeyExW, currentModule.hModule);
			ReplaceStandartFunctionToHoockFunction("Advapi32.dll", GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegOpenKeyExW") , (PROC)MyRegOpenKeyExW, currentModule.hModule);
			ReplaceStandartFunctionToHoockFunction("Advapi32.dll", GetProcAddress(GetModuleHandleA("Advapi32.dll"), "RegCloseKey"), (PROC)MyRegCloseKey, currentModule.hModule);
		} 
		while (Module32Next(snapOfModules, &currentModule));
	}
}


