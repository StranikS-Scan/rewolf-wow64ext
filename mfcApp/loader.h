#include <Windows.h>
#include <Winternl.h>
#ifndef _WIN64
typedef DWORD DWORDX;
#else
typedef DWORD64 DWORDX;
#endif

typedef NTSTATUS(WINAPI *_NtCreateThreadEx)(PHANDLE ThreadHandle, 
	ACCESS_MASK DesiredAccess, LPVOID ObjectAttributes, HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, 
	BOOL CreateSuspended, DWORD64 dwStackSize, 
	DWORD64 dw1, DWORD64 dw2, LPVOID Unknown);
typedef NTSTATUS(WINAPI *LdrGetProcedureAddressT)(IN PVOID DllHandle,
	IN PANSI_STRING ProcedureName OPTIONAL,IN ULONG ProcedureNumber OPTIONAL,
	OUT FARPROC *ProcedureAddress);
typedef VOID (WINAPI *RtlFreeUnicodeStringT)(_Inout_ PUNICODE_STRING UnicodeString);
typedef VOID (WINAPI *RtlInitAnsiStringT)(_Out_ PANSI_STRING DestString,
	_In_opt_ PCSZ SourceString);
typedef NTSTATUS (WINAPI *RtlAnsiStringToUnicodeStringT)(_Inout_ PUNICODE_STRING DestString,
	_In_ PCANSI_STRING SourceString,_In_ BOOLEAN AllocateDestinationString);
typedef NTSTATUS(WINAPI *LdrLoadDllT)(PWCHAR, PULONG, PUNICODE_STRING, PHANDLE);
typedef BOOL (APIENTRY *ProcDllMain)(LPVOID, DWORD, LPVOID );
typedef NTSTATUS (WINAPI *NtAllocateVirtualMemoryT)(IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,IN ULONG ZeroBits,IN OUT PSIZE_T RegionSize,
	IN ULONG AllocationType,IN ULONG Protect);

struct PARAMX
{
	PVOID lpFileData;
	DWORDX DataLength;
	LdrGetProcedureAddressT LdrGetProcedureAddress;
	NtAllocateVirtualMemoryT dwNtAllocateVirtualMemory;
	LdrLoadDllT pLdrLoadDll;
	RtlInitAnsiStringT RtlInitAnsiString;
	RtlAnsiStringToUnicodeStringT RtlAnsiStringToUnicodeString;
	RtlFreeUnicodeStringT RtlFreeUnicodeString;	
};

struct PARAMX64
{
	DWORD64 lpFileData;
	DWORD64 DataLength;
	DWORD64 LdrGetProcedureAddress;
	DWORD64 dwNtAllocateVirtualMemory;
	DWORD64 pLdrLoadDll;
	DWORD64 RtlInitAnsiString;
	DWORD64 RtlAnsiStringToUnicodeString;
	DWORD64 RtlFreeUnicodeString;
};

BOOL   EnableDebugPrivilege1();

DWORD_PTR WINAPI MemLoadLibrary2(PARAMX *X);
void SaveShellCode(int size);
void LoadLocalDll(LPSTR dllName);
void LoadLocalData(LPVOID data, DWORD dataSize);

#ifdef _WIN64
BOOL LoadRemoteDataX64ByX64(LPVOID data, DWORD dataSize, DWORD processId);
#else
BOOL LoadRemoteData32By64(LPVOID data, DWORD dataSize, DWORD processId);
VOID LoadRemoteData32by32(LPVOID data, DWORD dataSize, DWORD processId);
BOOL LoadLocalData32By64(LPVOID data, DWORD dataSize);
#endif









































































