// FilterSetDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FilterSetDlg.h"


// CFilterSetDlg ダイアログ

IMPLEMENT_DYNAMIC(CFilterSetDlg, CDialog)

CFilterSetDlg::CFilterSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterSetDlg::IDD, pParent)
	, strSrcHost(_T(""))
	, strDstHost(_T(""))
	, strSrcNet(_T(""))
	, strDstNet(_T(""))
	, strSrcPort(_T(""))
	, strDstPort(_T(""))
{

}

CFilterSetDlg::~CFilterSetDlg()
{
}

void CFilterSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC_HOST, strSrcHost);
	DDX_Text(pDX, IDC_EDIT_DST_HOST, strDstHost);
	DDX_Text(pDX, IDC_EDIT_SRC_NET, strSrcNet);
	DDX_Text(pDX, IDC_EDIT_DST_NET, strDstNet);
	DDX_Text(pDX, IDC_EDIT_SRC_PORT, strSrcPort);
	DDX_Text(pDX, IDC_EDIT_DST_PORT, strDstPort);
}


BEGIN_MESSAGE_MAP(CFilterSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFilterSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterSetDlg メッセージ ハンドラ


/*************************
// フィルタリングの適用
*************************/
void CFilterSetDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	m_stFilterData.strSrcHost	= strSrcHost;
	m_stFilterData.strDstHost	= strDstHost;
	m_stFilterData.strSrcIP		= strSrcNet;
	m_stFilterData.strDstIP		= strDstNet;
	m_stFilterData.strSrcPort	= strSrcPort;
	m_stFilterData.strDstPort	= strDstPort;
	
	OnOK();
}
