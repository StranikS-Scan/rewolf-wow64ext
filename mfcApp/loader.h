#include <Windows.h>
#include <Winternl.h>
#ifndef _WIN64
typedef DWORD DWORDX;
#else
typedef DWORD64 DWORDX;
#endif

typedef NTSTATUS(WINAPI*NtCreateThreadEx)(PHANDLE ThreadHandle,
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
	DWORDX nDataLength;
	LdrGetProcAddrT fnLdrGetProcAddr;
	NtAllocVirtualMemT fnNtAllocVirtualMem;
	LdrLoadDllT fnLdrLoadDll;
	RtlInitAnsiStrT fnRtlInitAnsiString;
	RtlAnsiStrToUniStrT fnRtlAnsiStriToUniStr;
	RtlFreeUniStrT fnRtlFreeUniStr;	
};

struct PARAMX64
{
	DWORD64 lpFileData;
	DWORD64 nDataLength;
	DWORD64 fnLdrGetProcAddr;
	DWORD64 fnNtAllocVirtualMem;
	DWORD64 fnLdrLoadDll;
	DWORD64 fnRtlInitAnsiString;
	DWORD64 fnRtlAnsiStriToUniStr;
	DWORD64 fnRtlFreeUniStr;
};

//打开DEBUG调试权限，有用到
BOOL EnableDebugPrivilege();

DWORD_PTR WINAPI MemLoadLibrary(PARAMX *X);
//32位下编译，启动32位DLL成功，包括控制台DLL和MFC的DLL
//64位下编译，启动64位DLL成功，包括控制台DLL和MFC的DLL
//如果编译时的位数与DLL的位数不一致，直接返回失败
BOOL LoadLocalData(LPVOID data, DWORD dataSize);

#ifndef _WIN64
//该函数没有任何效果，加载32位DLL会失败，加载64位DLL会崩溃
BOOL LoadLocalData32By64(LPVOID data, DWORD dataSize);
//Run-Time Check Failure #0，The value of ESP was not properly saved
BOOL LoadRemoteData32By32(LPVOID data, DWORD dataSize, DWORD processId);
//该函数没有任何效果
BOOL LoadRemoteData32By64(LPVOID data, DWORD dataSize, DWORD processId);
//该函数没有任何效果
BOOL LoadRemoteData64By64(LPVOID data, DWORD dataSize, DWORD processId);
#else
//不支持，直接失败
BOOL LoadLocalData32By64(LPVOID data, DWORD dataSize);
BOOL LoadRemoteData32By64(LPVOID data, DWORD dataSize, DWORD processId);
//64位下编译，它能将64位的控制台DLL，注入到64位EXE中，并且运行成功
//	将64位的MFC的DLL，注入到64位EXE中，会导致EXE崩溃（无法处理）
//	将64位的控制台DLL，注入到32位EXE中，会导致EXE崩溃（直接失败）
//	将32位的控制台DLL，注入到32位EXE中，会导致EXE崩溃（直接失败）
//	将32位的控制台DLL，注入到64位EXE中，提示成功但没效果（直接失败）
BOOL LoadRemoteData64By64(LPVOID data, DWORD dataSize, DWORD processId);
#endif
BOOL IsValidData(LPVOID data, DWORD dataSize);
BOOL Is64BitProcess(HANDLE hProcess, BOOL& Is64Bit);
BOOL Is64BitOS(BOOL& Is64Bit);