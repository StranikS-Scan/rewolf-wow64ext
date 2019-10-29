#include <Windows.h>
#include <Winternl.h>
#ifndef _WIN64
typedef DWORD DWORDX;
#else
typedef DWORD64 DWORDX;
#endif

typedef NTSTATUS(WINAPI*_NtCreateThreadEx)(PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess, LPVOID ObjectAttributes, HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, 
	BOOL CreateSuspended, DWORD64 dwStackSize, 
	DWORD64 dw1, DWORD64 dw2, LPVOID Unknown);
typedef NTSTATUS(WINAPI *LdrGetProcAddrT)(IN PVOID DllHandle,
	IN PANSI_STRING ProcedureName OPTIONAL,IN ULONG ProcedureNumber OPTIONAL,
	OUT FARPROC *ProcedureAddress);
typedef VOID (WINAPI *RtlFreeUniStrT)(_Inout_ PUNICODE_STRING UnicodeString);
typedef VOID (WINAPI *RtlInitAnsiStrT)(_Out_ PANSI_STRING DestString,
	_In_opt_ PCSZ SourceString);
typedef NTSTATUS (WINAPI *RtlAnsiStrToUniStrT)(_Inout_ PUNICODE_STRING DestString,
	_In_ PCANSI_STRING SourceString,_In_ BOOLEAN AllocateDestinationString);
typedef NTSTATUS(WINAPI *LdrLoadDllT)(PWCHAR, PULONG, PUNICODE_STRING, PHANDLE);
typedef BOOL (APIENTRY *ProcDllMain)(LPVOID, DWORD, LPVOID );
typedef NTSTATUS (WINAPI *NtAllocVirtualMemT)(IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,IN ULONG ZeroBits,IN OUT PSIZE_T RegionSize,
	IN ULONG AllocationType,IN ULONG Protect);

struct PARAMX
{
	PVOID lpFileData;
	DWORDX DataLength;
	LdrGetProcAddrT LdrGetProcAddr;
	NtAllocVirtualMemT dwNtAllocVirtualMem;
	LdrLoadDllT pLdrLoadDll;
	RtlInitAnsiStrT RtlInitAnsiString;
	RtlAnsiStrToUniStrT RtlAnsiStriToUniStr;
	RtlFreeUniStrT RtlFreeUniStr;	
};

struct PARAMX64
{
	DWORD64 lpFileData;
	DWORD64 DataLength;
	DWORD64 LdrGetProcAddr;
	DWORD64 dwNtAllocVirtualMem;
	DWORD64 pLdrLoadDll;
	DWORD64 RtlInitAnsiString;
	DWORD64 RtlAnsiStriToUniStr;
	DWORD64 RtlFreeUniStr;
};

//打开DEBUG调试权限，有用到
BOOL EnableDebugPrivilege();

DWORD_PTR WINAPI MemLoadLibrary(PARAMX *X);
BOOL SaveShellCode(DWORD size); //没有用到
BOOL LoadLocalData(LPVOID data, DWORD dataSize);

#ifdef _WIN64
BOOL LoadRemoteData64By64(LPVOID data, DWORD dataSize, DWORD processId);
#else
//该函数没有任何效果
BOOL LoadLocalData32By64(LPVOID data, DWORD dataSize);
//该函数没有任何效果
BOOL LoadRemoteData32By64(LPVOID data, DWORD dataSize, DWORD processId);
//Run-Time Check Failure #0，The value of ESP was not properly saved
BOOL LoadRemoteData32By32(LPVOID data, DWORD dataSize, DWORD processId);
//该函数没有任何效果
BOOL LoadRemoteData64By64(LPVOID data, DWORD dataSize, DWORD processId);
#endif









































































