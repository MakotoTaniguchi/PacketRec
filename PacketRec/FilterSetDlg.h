#ifndef	__FILTERSETDLG_H__
#define	__FILTERSETDLG_H__

#pragma once

#include "WinpcapCtrl.h"

// CFilterSetDlg ダイアログ

class CFilterSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CFilterSetDlg)

public:
	CFilterSetDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CFilterSetDlg();

// ダイアログ データ
	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	FILTERDATA GetFilterData(){ return m_stFilterData; }

private:
	CString strSrcHost;	// 送信側コンピュータ名
	CString strDstHost;	// 受信側コンピュータ名
	CString strSrcNet;	// 送信側IP
	CString strDstNet;	// 受信側IP
	CString strSrcPort;	// 送信側ポート
	CString strDstPort;	// 送信側ポート

	FILTERDATA m_stFilterData;
};

#endif