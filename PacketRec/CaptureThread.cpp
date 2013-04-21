#include "StdAfx.h"
#include "CaptureThread.h"

/*******************
// コンストラクタ
*******************/
CCaptureThread::CCaptureThread(void) : m_lpzFilePath(NULL)
{
	m_strAdopterName = _T("");
	if( NULL == m_lpzFilePath )
	{
		m_lpzFilePath = new TCHAR[MAX_PATH];
	}
}

/*******************
// デストラクタ
*******************/
CCaptureThread::~CCaptureThread(void)
{
	if( NULL != m_lpzFilePath )
	{
		delete m_lpzFilePath;
	}
}

/*******************
// スレッド初期化
*******************/
void CCaptureThread::Initialize()
{
	m_WinCapCtrl.Initialize( (LPTSTR)m_strAdopterName.c_str() );
}

/*****************
// スレッド開始
*****************/
UINT CCaptureThread::ThreadMain()
{
	LOGBASE( NORMAL, _T("キャプチャースレッド開始 スレッドID:%d"), m_uThreadID );

	// カレントパス
	::GetModuleFileName( NULL, m_lpzFilePath, MAX_PATH );
	::PathRemoveFileSpec( m_lpzFilePath );

	// pcapファイルのファイル名作成
	TCHAR szFileName[MAX_PATH];
	SYSTEMTIME sysTime;
	::GetLocalTime( &sysTime );
	_stprintf( szFileName, _T("%04d%02d%02d_%02d%02d%03d.pcap"), 
								sysTime.wYear, sysTime.wMonth, sysTime.wDay,
								sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds );

	::PathAppend( m_lpzFilePath, szFileName );
	LOGBASE( NORMAL, _T("ファイルパスの設定 %s"), m_lpzFilePath );
	
	m_WinCapCtrl.CaptureStart( m_lpzFilePath );

	return 0;
}

/*****************
// スレッド停止
*****************/
BOOL CCaptureThread::StopThread( DWORD dwTimeOut /*= INFINITE*/ )
{
	m_WinCapCtrl.CaptureStop();

	LOGBASE( NORMAL, _T("キャプチャースレッド停止 スレッドID:%d"), m_uThreadID );

	return TRUE;
}