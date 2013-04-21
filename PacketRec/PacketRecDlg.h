
// PacketRecDlg.h : ヘッダー ファイル
//
#include "WinpcapCtrl.h"
#include "CaptureThread.h"

#pragma once

// CPacketRecDlg ダイアログ
class CPacketRecDlg : public CDialog
{
// コンストラクション
public:
	CPacketRecDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_PACKETMONITOR_DIALOG };
	CButton		m_BtnCaptureStart;
	CButton		m_BtnCaptureStop;
	CComboBox	m_CombAdopter;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// ファイルパス
	CString m_strFilePath;

	// アダプタリスト
	VEC_DEVICEDATA m_vecDeviceData;

	// キャプチャスレッド
	std::map<CString, CCaptureThread*> m_mapCaptureThread;
	//CCaptureThread m_CaptureThread;

public:
	/*******************
	// キャプチャ開始
	*******************/
	afx_msg void OnBnClickedCaptureStart();

	/*******************
	// キャプチャ停止
	*******************/
	afx_msg void OnBnClickedCaptureStop();

	/*******************
	// フィルタ設定
	*******************/
	afx_msg void OnBnClickedFilterSet();
};
