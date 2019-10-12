#pragma once
#include <Windows.h>
#include <tchar.h>

PSTR ConvertWSTRtoSTR(PCWSTR pStrIn);
PWSTR ConvertSTRtoWSTR(PCSTR pStrIn);
PSTR CopySTR(PCSTR pStrIn);
PSTR ConvertTSTRtoSTR(PCTSTR pStrIn);
PTSTR ConvertSTRtoTSTR(PCSTR pStrIn);