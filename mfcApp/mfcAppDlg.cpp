
// mfcAppDlg.cpp: 实现文件
#include "pch.h"
#include "framework.h"
#include "mfcApp.h"
#include "mfcAppDlg.h"
#include "afxdialogex.h"
#include "CommonLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CmfcAppDlg 对话框
CmfcAppDlg::CmfcAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRet = NULL;
}

void CmfcAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmfcAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CmfcAppDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_RUN_EXE, &CmfcAppDlg::OnBnClickedBtnRunExe)
	ON_BN_CLICKED(IDC_BTN_LOAD_DLL, &CmfcAppDlg::OnBnClickedBtnLoadDll)
	ON_BN_CLICKED(IDC_BTN_LOAD_LIB, &CmfcAppDlg::OnBnClickedBtnLoadLib)
	ON_BN_CLICKED(IDC_BTN_START_EXE, &CmfcAppDlg::OnBnClickedBtnStartExe)
	ON_BN_CLICKED(IDC_BTN_START_EXE2, &CmfcAppDlg::OnBnClickedBtnStartExe2)
	ON_BN_CLICKED(IDC_BTN_LOAD_DLL2, &CmfcAppDlg::OnBnClickedBtnLoadDll2)
	ON_BN_CLICKED(IDC_BTN_INJECT_DLL64, &CmfcAppDlg::OnBnClickedBtnInjectDll64)
END_MESSAGE_MAP()

#ifdef _DEBUG
#include <conio.h>
void InitConsoleWindow()
{
	AllocConsole();
	_cprintf("_cprintf()\r\n");
}
#endif

// CmfcAppDlg 消息处理程序
BOOL CmfcAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

#ifdef _DEBUG
	InitConsoleWindow();  // add
#endif
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	//PCTSTR text =TEXT("D:\\mywork\\rewolf-wow64ext\\bin\\conDll64.dll");
	//PCTSTR text = TEXT("D:\\mywork\\rewolf-wow64ext\\bin\\conDll32.dll");
	PCTSTR text = TEXT("..\\bin\\conDll32.dll");
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FILENAME);//获取相应的编辑框ID
	pEdit->SetWindowText(text); //设置默认显示的内容 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmfcAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CmfcAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CmfcAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

string CmfcAppDlg::getFileName()
{
	CString str;
	GetDlgItemText(IDC_EDIT_FILENAME, str);
	if (!str.IsEmpty())
	{
		PSTR pStr = ConvertTSTRtoSTR(str);
		string str (pStr);
		delete[]pStr;
		return str;
	}
	return "";
}

void CmfcAppDlg::OnBnClickedBtnOpenFile()
{
	//AfxMessageBox(_T("测试"));
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0"); // 文件过滤
	ofn.lpstrInitialDir = _T("D:\\"); // 默认文件目录
	ofn.lpstrFile = szBuffer;
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	BOOL bSel = GetOpenFileName(&ofn);
	if (bSel)
	{
		//::MessageBox(NULL, szBuffer, TEXT("SignalSelect"), MB_OK);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FILENAME);//获取相应的编辑框ID
		pEdit->SetWindowText(szBuffer); //设置默认显示的内容 
		/*HMODULE hDll = LoadLibrary(szBuffer);
		if (hDll) {
			FreeLibrary(hDll);
		}*/
	}
}

void CmfcAppDlg::OnBnClickedBtnLoadLib()
{
	string fileName = getFileName();
	HMODULE hDll = LoadLibraryA(fileName.c_str());
	if (hDll) 
	{
		//FreeLibrary(hDll); //不能释放，否则会崩溃
		::MessageBoxA(NULL, "成功", "加载Lib", MB_OK);
	}
	else
	{
		::MessageBoxA(NULL, "失败", "加载Lib", MB_OK);
	}
}

void CmfcAppDlg::OnBnClickedBtnStartExe()
{//cmd /c start *.exe
	string fileName = getFileName();
	//m_bRet = runExeFromFile(fileName);
	if (m_bRet > 0)
	{
		::MessageBoxA(NULL, "成功", "运行EXE", MB_OK);
	}
	else
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:0x%x"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("运行EXE"), MB_OK);
	}
}

void CmfcAppDlg::OnBnClickedBtnStartExe2()
{//CreateProcessA *.exe
	string fileName = getFileName();
	//m_bRet = runExeFromFile2(fileName);
	if (m_bRet > 0)
	{
		::MessageBoxA(NULL, "成功", "运行EXE2", MB_OK);
	}
	else
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:0x%x"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("运行EXE2"), MB_OK);
	}
}

void CmfcAppDlg::OnBnClickedBtnRunExe()
{
	char* buf = NULL;
	string fileName = getFileName();
	//int len = readFile(fileName, buf);
	//DWORD m_bRet = runExeFromMem(buf, len);
	if(m_bRet == 0)
	{
		::MessageBoxA(NULL, "成功", "内存运行EXE", MB_OK);
	}
	else 
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:%d"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("内存运行EXE"), MB_OK);
	}
	if (buf != NULL) 
	{
		delete[]buf;
	}
}

void CmfcAppDlg::OnBnClickedBtnLoadDll()
{//regsvr32 /s *.dll
	string fileName = getFileName();
	m_bRet = loadDllFromFile(fileName);
	if (m_bRet > 0)
	{
		::MessageBoxA(NULL, "成功", "加载DLL", MB_OK);
	}
	else
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:0x%x"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("加载DLL"), MB_OK);
	}
}

#include "loader.h"
void CmfcAppDlg::OnBnClickedBtnLoadDll2()
{//内存加载
	char* buf = NULL;
	string fileName = getFileName();
	int len = readFile(fileName, buf);
	//m_bRet = LoadLocalDll(fileName.c_str());
	m_bRet = LoadLocalData(buf, len);
	if (m_bRet > 0)
	{
		::MessageBoxA(NULL, "成功", "加载DLL", MB_OK);
	}
	else
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:0x%x"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("加载DLL"), MB_OK);
	}
	if (buf != NULL)
	{
		delete[]buf;
	}
}

void CmfcAppDlg::OnBnClickedBtnInjectDll64()
{//远程注入，64位注入64位
	char* buf = NULL;
	string fileName = getFileName();
	int len = readFile(fileName, buf);
	CString str;
	GetDlgItemText(IDC_EDIT_PROC_ID, str);
	DWORD pid = _ttoi(str);
	m_bRet = LoadRemoteDataX64ByX64(buf, len, pid);
	if (m_bRet > 0)
	{
		::MessageBoxA(NULL, "成功", "远程注入", MB_OK);
	}
	else
	{
		CString str;
		str.Format(TEXT("失败 ErrorID:0x%x"), m_bRet);
		::MessageBox(NULL, str.GetString(), TEXT("远程注入"), MB_OK);
	}
	if (buf != NULL)
	{
		delete[]buf;
	}
}
