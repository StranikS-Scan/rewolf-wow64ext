//ProcessManager.h

#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Psapi.lib")

UINT GetProcessID(PCTSTR cPorcessName, UINT len);
BOOL CloseProcess(UINT unProcessID);

#endif