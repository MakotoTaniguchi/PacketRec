#pragma once
#include "Threadbase.h"
#include "WinpcapCtrl.h"

class CCaptureThread :
	public CThreadBase
{
public:
	// コンストラクタ
	CCaptureThread(void);

	// デストラクタ
	~CCaptureThread(void);

	void Initialize();
	
	// スレッド停止
	BOOL StopThread( DWORD dwTimeOut = INFINITE );

	// スレッドメイン処理
	UINT ThreadMain();
	
	// アダプタ名設定
	void SetAdopterName( LPTSTR lpczAdopterName ){ m_strAdopterName = lpczAdopterName; }

	// ファイルパスの設定
	void setFilePath( LPTSTR lpzFilePath ){ m_lpzFilePath = lpzFilePath; }

	// フィルタの設定
	void SetFilterData( FILTERDATA stFilterData ){ m_WinCapCtrl.PacketFiltering( stFilterData ); }

public:
	// アダプタ名
	TSTRING m_strAdopterName;

	// ファイルパス
	LPTSTR m_lpzFilePath;

	// キャプチャコントロール
	CWinpcapCtrl m_WinCapCtrl;
};
