
// PacketMonitorDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PacketRec.h"
#include "PacketRecDlg.h"
#include "FilterSetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPacketRecDlg ダイアログ
CPacketRecDlg::CPacketRecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketRecDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPacketRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAPTURE_START, m_BtnCaptureStart);
	DDX_Control(pDX, IDC_CAPTURE_STOP, m_BtnCaptureStop);
	DDX_Control(pDX, IDC_COMBO_ADOPTER, m_CombAdopter);
}

BEGIN_MESSAGE_MAP(CPacketRecDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CAPTURE_START, &CPacketRecDlg::OnBnClickedCaptureStart)
	ON_BN_CLICKED(IDC_CAPTURE_STOP, &CPacketRecDlg::OnBnClickedCaptureStop)
END_MESSAGE_MAP()


// CPacketRecDlg メッセージ ハンドラ

BOOL CPacketRecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	m_vecDeviceData = CWinpcapCtrl::AdopterOpen();

	VEC_DEVICEDATA::iterator it;
	for( it = m_vecDeviceData.begin(); it != m_vecDeviceData.end(); it++ )
	{
		m_CombAdopter.AddString(it->strDeviceName.c_str());
	}

	LOGBASE( NORMAL, _T("画面初期化完了") );

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CPacketRecDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CPacketRecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CPacketRecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/****************************************************************//*!
 @brief		キャプチャー開始処理
 @details	詳細説明（省略可）
 @retval	なし
 @exception	例外出力（無ければ省略）
 @attention 注意書き（省略可）
 @note		
 @sa ------------------------------------------------------------
 - $Fnc>	CPacketRecDlg::OnBnClickedCaptureStart
 - $Prg>	0%
 - $Upd>	2012/05/30 谷口 UID 処理内容
 - $Tst>	2012/05/30 谷口 UID [SA] -- [AV] -- [UT] --
****************************************************************///
void CPacketRecDlg::OnBnClickedCaptureStart()
{
	CString strAdopter;
	m_CombAdopter.GetWindowText( strAdopter );

	VEC_DEVICEDATA::iterator it;
	for( it = m_vecDeviceData.begin(); it != m_vecDeviceData.end(); it++ )
	{
		if( 0 == strAdopter.Compare(it->strDeviceName.c_str()) )
		{
			strAdopter = it->strAdopter.c_str();
			break;
		}
	}

	if( m_mapCaptureThread.end() == m_mapCaptureThread.find( strAdopter ) )
	{
		CCaptureThread* pCaptureThread = new CCaptureThread();
		pCaptureThread->CreateThread(FALSE);

		/*
		CFilterSetDlg dlg;
		INT_PTR nRet = dlg.DoModal();
		if( IDOK == nRet )
		{
			pCaptureThread->SetFilterData( dlg.GetFilterData() );
		}
		*/
		pCaptureThread->SetAdopterName( (LPTSTR)strAdopter.GetString() );
		pCaptureThread->Initialize();
		LOGBASE( NORMAL, _T("アダプタの設定 %s"), (LPTSTR)strAdopter.GetString() );

		pCaptureThread->StartThread();
		m_mapCaptureThread[strAdopter] = pCaptureThread;
		LOGBASE( NORMAL, _T("キャプチャーが開始されました。") );
	}
}

/****************************************************************//*!
 @brief		キャップチャー停止処理
 @details	詳細説明（省略可）
 @retval	なし
 @exception	例外出力（無ければ省略）
 @attention 注意書き（省略可）
 @note		
 @sa ------------------------------------------------------------
 - $Fnc>	CPacketRecDlg::OnBnClickedCaptureStop
 - $Prg>	0%
 - $Upd>	2012/05/30 谷口 UID 処理内容
 - $Tst>	2012/05/30 谷口 UID [SA] -- [AV] -- [UT] --
****************************************************************///
void CPacketRecDlg::OnBnClickedCaptureStop()
{
	CString strAdopter;
	m_CombAdopter.GetWindowText( strAdopter );

	VEC_DEVICEDATA::iterator it;
	for( it = m_vecDeviceData.begin(); it != m_vecDeviceData.end(); it++ )
	{
		if( 0 == strAdopter.Compare(it->strDeviceName.c_str()) )
		{
			strAdopter = it->strAdopter.c_str();
			break;
		}
	}

	if( m_mapCaptureThread.end() != m_mapCaptureThread.find( strAdopter ) )
	{
		m_mapCaptureThread[strAdopter]->StopThread();
		m_mapCaptureThread[strAdopter]->Destroy();
		delete m_mapCaptureThread[strAdopter];
		m_mapCaptureThread.erase(strAdopter);
		LOGBASE( NORMAL, _T("キャプチャーが停止されました。") );
		::MessageBox(NULL, _T("終了します。"), _T("終了"), MB_OK);
	}
}
