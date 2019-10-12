//ProcessManager.cpp
//#include <winnt.h>
#include "ProcessManager.h"

UINT GetProcessID(PCTSTR cPorcessName, UINT len)
{
	PROCESSENTRY32 entry = { 0 };
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bRet = Process32First(hProcess, &entry);
	while (bRet)
	{
		if (0 == _tcscmp(entry.szExeFile, cPorcessName))
		{//�ҵ��󷵻ؽ���ID;
			return entry.th32ProcessID;
		}
		else
		{//������һ������
			bRet = Process32Next(hProcess, &entry);
		}
	}
	return 0;
}

BOOL CloseProcess(UINT unProcessID)
{
	HANDLE bExitCode = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION 
		| PROCESS_VM_WRITE | PROCESS_ALL_ACCESS, FALSE, unProcessID);
	if (NULL != bExitCode)
	{
		BOOL bFlag = TerminateProcess(bExitCode, 0);
		CloseHandle(bExitCode);
		return TRUE;
	}
	return FALSE;
}