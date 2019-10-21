// mfcAppDlg.h: 头文件
#pragma once
#include <string>
using namespace std;

// CmfcAppDlg 对话框
class CmfcAppDlg : public CDialogEx
{
// 构造
public:
	CmfcAppDlg(CWnd* pParent = nullptr); // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	BOOL m_bRet;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	string getFileName();
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnCreateDir();
	afx_msg void OnBnClickedBtnDelDir();
	afx_msg void OnBnClickedBtnGetIp();
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnBnClickedBtnDownload2();
	afx_msg void OnBnClickedBtnUnzip();
	afx_msg void OnBnClickedBtnGetCfg();
	afx_msg void OnBnClickedBtnGetMd5();
	afx_msg void OnBnClickedBtnStartExe();
	afx_msg void OnBnClickedBtnRunExe();
	afx_msg void OnBnClickedBtnLoadLib();
	afx_msg void OnBnClickedBtnLoadDll();
	afx_msg void OnBnClickedBtnGetSysDir();
	afx_msg void OnBnClickedBtnFile2array();
	afx_msg void OnBnClickedBtnRemoveExe();
	afx_msg void OnBnClickedBtnStartExe2();
	afx_msg void OnBnClickedBtnLoadDll2();
	afx_msg void OnBnClickedBtnInjectDll64();
};
