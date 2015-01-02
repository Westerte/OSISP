#include <wdm.h>

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject);

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, UnloadRoutine)

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	DbgPrint("Hello, my little friend, don't be sad");
	DriverObject->DriverUnload = UnloadRoutine;
	return STATUS_SUCCESS;
}

VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("Good luck!\n");
}
