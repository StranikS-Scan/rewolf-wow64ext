#include "PstrConverter.h"

PSTR ConvertWSTRtoSTR(PCWSTR pStrIn)
{
	LPSTR pStrOut = NULL;
	if (pStrIn != NULL)
	{
		int nInputStrLen = (int)wcslen(pStrIn);
		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, 
			pStrIn, nInputStrLen, NULL, 0, 0, 0) + 1;
		pStrOut = new CHAR[nOutputStrLen];
		if (pStrOut)
		{
			memset(pStrOut, 0x00, nOutputStrLen * sizeof(CHAR));
			nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, pStrIn,
				nInputStrLen, pStrOut, nOutputStrLen, 0, 0);
		}
	}
	return pStrOut;
}

PWSTR ConvertSTRtoWSTR(PCSTR pStrIn)
{
	LPWSTR pStrOut = NULL;
	if (pStrIn != NULL)
	{
		int nInputStrLen = (int)strlen(pStrIn);
		int nOutputStrLen = MultiByteToWideChar(CP_ACP, 0,
			pStrIn, nInputStrLen, NULL, 0) + 1;
		pStrOut = new WCHAR[nOutputStrLen];
		if (pStrOut)
		{
			memset(pStrOut, 0x00, nOutputStrLen * sizeof(WCHAR));
			nOutputStrLen = MultiByteToWideChar(CP_ACP, 0, pStrIn,
				nInputStrLen, pStrOut, nOutputStrLen);
		}
	}
	return pStrOut;
}

PSTR CopySTR(PCSTR pStrIn)
{
	PSTR pStrOut = NULL;
	int nInputStrLen = (int)strlen(pStrIn);
	int nOutputStrLen = nInputStrLen + 1;
	pStrOut = new CHAR[nOutputStrLen];
	if (pStrOut)
	{
		memset(pStrOut, 0x00, nOutputStrLen * sizeof(CHAR));
		memcpy(pStrOut, pStrIn, nInputStrLen);
	}
	return pStrOut;
}

PSTR ConvertTSTRtoSTR(PCTSTR pStrIn)
{
#ifdef UNICODE
	return ConvertWSTRtoSTR(pStrIn);
#else
	return CopySTR((PCSTR)pStrIn);
#endif
}

PTSTR ConvertSTRtoTSTR(PCSTR pStrIn)
{
#ifdef UNICODE
	return ConvertSTRtoWSTR(pStrIn);
#else
	return CopySTR(pStrIn);
#endif
}