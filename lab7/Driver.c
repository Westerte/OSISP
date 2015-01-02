#include <ntddk.h>

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
VOID CreateProcessNotifyRoutine(IN HANDLE ParentId, IN HANDLE ProcessId, IN BOOLEAN Create);
extern UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
extern NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);
char* GetProcessNameFromPid(HANDLE pid);
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject);

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, UnloadRoutine)
#pragma alloc_text(PAGE, CreateProcessNotifyRoutine)
#pragma alloc_text(PAGE, GetProcessNameFromPid)

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{		
	NTSTATUS status;
	DbgPrint("Hello, my little friend, don't be sad");
	status = PsSetCreateProcessNotifyRoutine(CreateProcessNotifyRoutine, FALSE);
	if (status == STATUS_SUCCESS)
	{
		DbgPrint("CreateProcessNotifyRoutine have been loaded");	
		DriverObject->DriverUnload = UnloadRoutine;
	}
	else
	{
		DbgPrint("We have problems with loading CreateProcessNotifyRoutine");	
	}
	return STATUS_SUCCESS;
}

char* GetProcessNameFromPid(HANDLE pid)
{	
	PEPROCESS Process;
	if (PsLookupProcessByProcessId(pid, &Process) == STATUS_INVALID_PARAMETER)
	{
		return "Some mistake";
	}
	return (CHAR*)PsGetProcessImageFileName(Process);
}

VOID CreateProcessNotifyRoutine(HANDLE hParentId, HANDLE hProcessId, BOOLEAN bCreate)
{
	char* processName = GetProcessNameFromPid(hProcessId); 
	if (bCreate)
	{	
		DbgPrint("Process %s have been created\n", processName);
		if (strcmp(processName, "notepad.exe") == 0)
		{	
			DbgPrint("%s detected, id = %d\n", processName, hProcessId);
		}
	}
	else 
	{	 
		DbgPrint("Process %s have been terminated\n", processName);
	}
}

VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject)
{
	PsSetCreateProcessNotifyRoutine(CreateProcessNotifyRoutine, TRUE);
	DbgPrint("Good luck!\n");
}
