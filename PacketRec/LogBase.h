#ifndef _LOGBASE_H_
#define _LOGBASE_H_

#pragma once

#include "tchar.h"
#include <string>
#include <stdarg.h>

typedef std::basic_string<TCHAR> TSTRING;

/*******************
// ログ出力開始
*******************/
#define LOGBASE_START	CLogBase(__FILE__, __LINE__).StartLog

/*******************
// ログ出力
*******************/
#define LOGBASE			CLogBase(__FILE__, __LINE__).Log

/*******************
// ログ出力終了
*******************/
#define LOGBASE_END		CLogBase(__FILE__, __LINE__).EndLog

enum ETYPE
{
	NORMAL = 0,		// 正常
	ABNORMAL =1		// 異常
};

/***********************
// ファイルパスクラス
***********************/
class CLogFilePath
{
public:
	CLogFilePath();
	~CLogFilePath();
	
	void operator =( LPTSTR lpzFilePath );
	
	LPTSTR GetLogFilePath(){ return m_lpzLogFilePath; }
	
private:
	LPTSTR m_lpzLogFilePath;
};

/************
// ログクラス
************/
class CLogBase
{
public:
	CLogBase( LPTSTR lpzFilePath = _T(""), DWORD dwLine = 0 );
	~CLogBase();

	void StartLog( LPTSTR lpzLogPath );
	void Log( ETYPE eType, LPTSTR lpzFormat, ... );
	void EndLog();

private:
	// 年月日\t時分秒\tファイル名\t行数
	int GetHeaderLog( LPTSTR lpzLogPath );

	// ファイル書き込み
	BOOL WriteLog( LPCTSTR lpzLog );

private:
	// ファイルハンドル
	HANDLE m_hFile;

	// クリティカルセクション
	CRITICAL_SECTION m_cs;

	// ファイルパス
	LPTSTR m_lpzFilePath;

	// ライン
	DWORD m_dwLine;
};

#endif // _LOGBASE_H_