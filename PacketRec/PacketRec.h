
// PacketMonitor.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPacketMonitorApp:
// このクラスの実装については、PacketMonitor.cpp を参照してください。
//

// CWinAppExからExを削るとサイズが小さくなります
class CPacketRecApp : public CWinApp
{
public:
	CPacketRecApp();
	~CPacketRecApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPacketRecApp theApp;