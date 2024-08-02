#include <ntifs.h>
#include <ntddk.h>
#include <ntdef.h>
#include <windef.h>
#include <ntimage.h>
#include <cstdint>
#include <cstddef>
#include <intrin.h>
#include <wingdi.h>
#include <ntstrsafe.h>
#pragma comment(lib, "ntoskrnl.lib")

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;  // in bytes
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;  // LDR_*
    USHORT LoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    PVOID SectionPointer;
    ULONG CheckSum;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef enum SYSTEM_INFORMATION_CLASS
{
    SystemInformationClassMin = 0,
    SystemBasicInformation = 0,
    SystemProcessorInformation = 1,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemPathInformation = 4,
    SystemNotImplemented1 = 4,
    SystemProcessInformation = 5,
    SystemProcessesAndThreadsInformation = 5,
    SystemCallCountInfoInformation = 6,
    SystemCallCounts = 6,
    SystemDeviceInformation = 7,
    SystemConfigurationInformation = 7,
    SystemProcessorPerformanceInformation = 8,
    SystemProcessorTimes = 8,
    SystemFlagsInformation = 9,
    SystemGlobalFlag = 9,
    SystemCallTimeInformation = 10,
    SystemNotImplemented2 = 10,
    SystemModuleInformation = 11,
    SystemLocksInformation = 12,
    SystemLockInformation = 12,
    SystemStackTraceInformation = 13,
    SystemNotImplemented3 = 13,
    SystemPagedPoolInformation = 14,
    SystemNotImplemented4 = 14,
    SystemNonPagedPoolInformation = 15,
    SystemNotImplemented5 = 15,
    SystemHandleInformation = 16,
    SystemObjectInformation = 17,
    SystemPageFileInformation = 18,
    SystemPagefileInformation = 18,
    SystemVdmInstemulInformation = 19,
    SystemInstructionEmulationCounts = 19,
    SystemVdmBopInformation = 20,
    SystemInvalidInfoClass1 = 20,
    SystemFileCacheInformation = 21,
    SystemCacheInformation = 21,
    SystemPoolTagInformation = 22,
    SystemInterruptInformation = 23,
    SystemProcessorStatistics = 23,
    SystemDpcBehaviourInformation = 24,
    SystemDpcInformation = 24,
    SystemFullMemoryInformation = 25,
    SystemNotImplemented6 = 25,
    SystemLoadImage = 26,
    SystemUnloadImage = 27,
    SystemTimeAdjustmentInformation = 28,
    SystemTimeAdjustment = 28,
    SystemSummaryMemoryInformation = 29,
    SystemNotImplemented7 = 29,
    SystemNextEventIdInformation = 30,
    SystemNotImplemented8 = 30,
    SystemEventIdsInformation = 31,
    SystemNotImplemented9 = 31,
    SystemCrashDumpInformation = 32,
    SystemExceptionInformation = 33,
    SystemCrashDumpStateInformation = 34,
    SystemKernelDebuggerInformation = 35,
    SystemContextSwitchInformation = 36,
    SystemRegistryQuotaInformation = 37,
    SystemLoadAndCallImage = 38,
    SystemPrioritySeparation = 39,
    SystemPlugPlayBusInformation = 40,
    SystemNotImplemented10 = 40,
    SystemDockInformation = 41,
    SystemNotImplemented11 = 41,
    SystemInvalidInfoClass2 = 42,
    SystemProcessorSpeedInformation = 43,
    SystemInvalidInfoClass3 = 43,
    SystemCurrentTimeZoneInformation = 44,
    SystemTimeZoneInformation = 44,
    SystemLookasideInformation = 45,
    SystemSetTimeSlipEvent = 46,
    SystemCreateSession = 47,
    SystemDeleteSession = 48,
    SystemInvalidInfoClass4 = 49,
    SystemRangeStartInformation = 50,
    SystemVerifierInformation = 51,
    SystemAddVerifier = 52,
    SystemSessionProcessesInformation = 53,
    SystemInformationClassMax
} SYSTEM_INFORMATION_CLASS;

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR  FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;


extern "C" {
    NTSTATUS
        WINAPI ZwQuerySystemInformation(
            _In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
            _Inout_   PVOID                    SystemInformation,
            _In_      ULONG                    SystemInformationLength,
            _Out_opt_ PULONG                   ReturnLength
        );
    NTSTATUS
        MmCopyVirtualMemory(
            IN  PEPROCESS FromProcess,
            IN  CONST VOID* FromAddress,
            IN  PEPROCESS ToProcess,
            OUT PVOID ToAddress,
            IN  SIZE_T BufferSize,
            IN  KPROCESSOR_MODE PreviousMode,
            OUT PSIZE_T NumberOfBytesCopied
        );
    NTKERNELAPI
        PVOID
        NTAPI
        RtlFindExportedRoutineByName(
            _In_ PVOID ImageBase,
            _In_ PCCH RoutineNam
        );
}

struct WindowStyle
{
    char padding_1[0x18];		// 0x18
    ULONG exStyle;
    ULONG style;
};

struct Win32Thread
{
    void* pThread;				// 0x0
};

typedef struct _TAG_WND
{
    HWND hWnd;					// 0x0
    UINT64 windowCount;			// 0x8
    Win32Thread* pWin32Thread;	// 0x10
    char padding_1[0x10];		// 0x18
    WindowStyle* windowStyle;	// 0x28
    UINT64 userCopyHeapOffset;	// 0x30
    char padding_2[0x20];
    _TAG_WND* next;				// 0x58	
    _TAG_WND* previous;			// 0x60
    _TAG_WND* parent;			// 0x68
_TAG_WND* child;			// 0x70
char padding_3[0x40];
PWCH* windowName;			// 0xB8
char padding_4[0x8];
_TAG_WND* self;				// 0xC8
}TAG_WND, * PTAG_WND;

struct mydata {
    ULONG handle;
};



void unload(PDRIVER_OBJECT DriverObject) {
    KdPrint(("Driver unloaded lmao"));
    UNICODE_STRING syml = RTL_CONSTANT_STRING(L"\\??\\pogk");
    IoDeleteSymbolicLink(&syml);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS CompleteIrp(PIRP IRP, NTSTATUS status, int information) {
    IRP->IoStatus.Status = status;
    IRP->IoStatus.Information = information;
    IoCompleteRequest(IRP, IO_NO_INCREMENT);
    return status;
}

NTSTATUS CreateClose(PDEVICE_OBJECT DeviceObject, PIRP IRP) {
    return CompleteIrp(IRP, STATUS_SUCCESS, 0);
}

void* FindModule(const char* name) {
    ULONG bytes = 0;
    void* win32k = nullptr;
    auto status = ZwQuerySystemInformation(SystemModuleInformation, NULL, NULL, &bytes);
    if (!bytes) {
        KdPrint(("Failed to query info 1\n"));
        KdPrint(("%lld\n", (long long)status));
        return nullptr;
    }
    PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 'kekk');
    if (!modules) {
        KdPrint(("Failed to allocate pool\n"));
        return nullptr;
    }
    status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);
    void* moduleptr = nullptr;
    if (NT_SUCCESS(status)) {
        for (ULONG i = 0; i < modules->NumberOfModules; ++i) {
            if (_stricmp((char*)modules->Modules[i].FullPathName, name) == 0) {
                moduleptr = modules->Modules[i].ImageBase;
                break;
            }
        }
    }
    else {
        KdPrint(("Failed to query info 2\n"));
    }
    ExFreePoolWithTag(modules, 'kekk');
    return moduleptr;
}

using ValidateHwnd_t = TAG_WND * (*)(ULONGLONG);
NTSTATUS HideWindow(mydata d) {
    static bool init = true;
    static ValidateHwnd_t ValidateHwnd = nullptr;
    if (init) {
        auto res = FindModule("\\SystemRoot\\System32\\win32kbase.sys");
        if (!res) {
            KdPrint(("Failed to find win32k\n"));
            return STATUS_DATA_ERROR;
        }
        ValidateHwnd = (ValidateHwnd_t)RtlFindExportedRoutineByName(res, "ValidateHwnd");
        if (!ValidateHwnd) {
            KdPrint(("Failed to find ValidateHwnd\n"));
            return STATUS_DATA_ERROR;
        }
        init = false;
    }
    auto tagwndptr = ValidateHwnd(d.handle);
    if (tagwndptr == nullptr) {
        KdPrint(("null from validatehwnd\n"));
        return STATUS_DATA_ERROR;
    }
    KdPrint(("tagwndptr gotten %p\n", tagwndptr));
    BYTE* ptr = (BYTE*)tagwndptr->windowStyle;
    if (!ptr) {
        KdPrint(("ptr is null\n"));
        return STATUS_DATA_ERROR;
    }
    (*(DWORD*)(ptr + 42)) |= 2;
    tagwndptr->windowStyle->style |= 1;
    tagwndptr->windowStyle->exStyle |= 1;
    *(ptr + 0x31) &= (255 - 16);
    return STATUS_SUCCESS;
}

NTSTATUS DeviceControl(PDEVICE_OBJECT DeviceObject, PIRP IRP) {
    auto stack = IoGetCurrentIrpStackLocation(IRP);
    auto code = stack->Parameters.DeviceIoControl;
    NTSTATUS status = STATUS_SUCCESS;
    switch (code.IoControlCode) {
        case 0x777:
        {
            if (code.InputBufferLength < sizeof(mydata)) {
                status = STATUS_INVALID_PARAMETER;
                break;
            }
            auto ptr = (mydata*)code.Type3InputBuffer;
            KdPrint(("hwnd gotten %uld\n", ptr->handle));
            if (ptr == nullptr) {
                status = STATUS_INVALID_PARAMETER;
                break;
            }
            status = HideWindow(*ptr);
            break;
        }
        default:
            status = STATUS_INVALID_PARAMETER;
    }
    return CompleteIrp(IRP, status, 0);
}


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    KdPrint(("Driver loaded lmao\n"));
    DriverObject->DriverUnload = unload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;
    UNICODE_STRING name;
    RtlInitUnicodeString(&name, L"\\Device\\pogk");
    PDEVICE_OBJECT DeviceObject;
    auto status = IoCreateDevice(DriverObject, 0, &name, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Very sad meow"));
        return STATUS_FAILED_DRIVER_ENTRY;
    }
    UNICODE_STRING symlink = RTL_CONSTANT_STRING(L"\\??\\pogk");
    status = IoCreateSymbolicLink(&symlink, &name);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Very sad meow symlink"));
        IoDeleteDevice(DeviceObject);
        return STATUS_FAILED_DRIVER_ENTRY;
    }
    return STATUS_SUCCESS;
}