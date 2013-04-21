#include "stdafx.h"
#include "WinpcapCtrl.h"

/*******************
// コンストラクタ
*******************/
CWinpcapCtrl::CWinpcapCtrl() : m_pPcap(NULL), m_bCancel(FALSE), m_hCancel(NULL)
{
	m_hCancel = ::CreateEvent( NULL, FALSE, FALSE, NULL );
}

/*******************
// デストラクタ
*******************/
CWinpcapCtrl::~CWinpcapCtrl()
{
}

/*******************
// オープンアダプタ
*******************/
VEC_DEVICEDATA CWinpcapCtrl::AdopterOpen()
{
	VEC_DEVICEDATA vecDeviceName;
	pcap_if_t *AddDevs = NULL;
	LPTSTR szErrorBuf = NULL;
	if( pcap_findalldevs(&AddDevs, szErrorBuf) != -1 )
	{
		TSTRING strIPAddress;
		pcap_if_t *d = AddDevs;
		while( d )
		{
			// IPアドレスの情報が有効なものだけをターゲットにする
			if( d->addresses != NULL ) {
				// sockaddr_inにキャストする
				sockaddr_in *sa = (sockaddr_in*)d->addresses->addr;

				// 現在有効でないデバイスは、
				// アドレスが0になっているので排除する
				//if( sa->sin_addr.s_addr != 0 )
				//{ 
					// sa->sin_addr.s_addr にIPアドレス
					// d->name にアダプタ名
					// d->descriptionにドライバ名
					strIPAddress.append( inet_ntoa( sa->sin_addr ) );
					CONNECTDEVICEDATA DeviceData = { d->description, d->name, strIPAddress };

					vecDeviceName.push_back( DeviceData );
				//}
				d = d->next;
			}
			else
			{
				LOGBASE( ABNORMAL, _T("IPアドレスが有効ではありません") );
				break;
			}
		}
		// デバイス情報のバッファを開放する
		pcap_freealldevs(AddDevs);
	}
	else
	{
		LOGBASE( ABNORMAL, _T("アダプタ取得失敗 %s"), szErrorBuf );
	}

	return vecDeviceName;
}

/*******************
// キャプチャ開始
*******************/
void CWinpcapCtrl::CaptureStart( LPTSTR lpzFilePath )
{
	// Pcapファイルオープン
	pcap_dumper_t* pDF = ::pcap_dump_open( m_pPcap, lpzFilePath );

	// パケットキャプチャ
	if( NULL != pDF )
	{
		while( !m_bCancel )
		{
			const u_char* pData;
			struct pcap_pkthdr* pkt_header;
			int dRet = ::pcap_next_ex( m_pPcap , &pkt_header, &pData );

			if( 0 < dRet )
			{
				LOGBASE( NORMAL, _T("パケット書き込み") );
				::pcap_dump( (u_char*)pDF, pkt_header, pData );
			}
		}
		::pcap_dump_close( pDF );
	}
	else
	{
		LOGBASE( NORMAL, _T("pcapファイルオープン失敗") );
	}

	::SetEvent( m_hCancel );
}

/*******************
// キャプチャ停止
*******************/
void CWinpcapCtrl::CaptureStop( DWORD dwWaite )
{
	if( m_bCancel )
	{
		return;
	}

	m_bCancel = TRUE;

	DWORD dwRet = ::WaitForSingleObject( m_hCancel, dwWaite );

	if( WAIT_OBJECT_0 != dwRet )
	{
		LOGBASE( ABNORMAL, _T("キャプチャー停止失敗 %ld"), dwRet );
	}

	if( NULL == m_pPcap )
	{
		LOGBASE( ABNORMAL, _T("クローズ処理不要") );
		return;
	}

	::pcap_close( m_pPcap );
}

/***************************
// パケットフィルタリング
***************************/
void CWinpcapCtrl::PacketFiltering( FILTERDATA stFilterData )
{
	if( NULL == m_pPcap )
	{
		return;
	}

	// フィルタの文字列作成
	TSTRING strFilterChar = _T("");
	strFilterChar.append( MakeFilterChar( stFilterData.strSrcHost,	stFilterData.strDstHost,	_T("host") ) );
	strFilterChar.append( MakeFilterChar( stFilterData.strSrcIP,	stFilterData.strDstIP,		_T("net") ) );
	strFilterChar.append( MakeFilterChar( stFilterData.strSrcPort,	stFilterData.strDstPort,	_T("port") ) );

	LOGBASE( ABNORMAL, _T("フィルタ文字列：%s\n"), strFilterChar.c_str() );

	char errbuf[PCAP_ERRBUF_SIZE];	/* エラー理由格納用文字配列 */
	bpf_u_int32 mask;				/* 監視ディバイスのネットマスク */
	bpf_u_int32 net;				/* 監視ディバイスのIPアドレス */

	// ネットマスク作成
	int nRet = ::pcap_lookupnet( m_strSelectAdopter.c_str(), &net, &mask, errbuf );
	if ( -1 == nRet) {
		LOGBASE( ABNORMAL, _T("ディバイスのネットマスクを取得できませんでした：%s\n"), m_strSelectAdopter.c_str() );
		net = 0;
		mask = 0;
	}

	bpf_program stBpfProg;			/* コンパイル済みフィルタ用構造体 */
	nRet = ::pcap_compile( m_pPcap, &stBpfProg, strFilterChar.c_str(), 1, net );
	if( -1 == nRet )
	{
		LOGBASE( ABNORMAL, _T("フィルタのコンパイルに失敗しました。") );
	}

	nRet = ::pcap_setfilter( m_pPcap, &stBpfProg );
	if( -1 == nRet )
	{
		LOGBASE( ABNORMAL, _T("フィルタの設定に失敗しました。") );
	}
}

/***************************
// パケット用文字列作成
***************************/
LPTSTR CWinpcapCtrl::MakeFilterChar( TSTRING strSrc, TSTRING strDst, TSTRING strType )
{
	// フィルタ文字列
	TSTRING strFilterChar = _T("");

	// 宛先元 or 宛先が0文字を超えていた場合
	if( 0 < strSrc.length() || 0 < strDst.length() )
	{
		if( 0 < strSrc.length() )
		{
			strFilterChar.append( _T("src") );
			strFilterChar.append( strType );
			strFilterChar.append( strSrc );
			strFilterChar.append( _T(" ") );
		}

		if( 0 < strDst.length() )
		{
			strFilterChar.append( _T("dst") );
			strFilterChar.append( strType );
			strFilterChar.append( strDst );
			strFilterChar.append( _T(" ") );
		}

		return (LPTSTR)strFilterChar.c_str();
	}

	return NULL;
}


/*****************************
// 収集対象アダプタ名の設定
*****************************/
void CWinpcapCtrl::Initialize( LPTSTR lpzAdopterName )
{
	// アダプタ設定
	m_strSelectAdopter = lpzAdopterName;

	if( m_strSelectAdopter.empty() )
	{
		LOGBASE( ABNORMAL, _T("アダプタが選択されていませんでした。") );
		return;
	}

	TCHAR szErrorBuf[1024];		// エラー文言
	int nTimeOut = 1000 * 5;	// タイムアウト値
	// プロミスキャスモードで収集
	// 65536byte溜まるまで制御を返さない
	if( ( m_pPcap = ::pcap_open_live( m_strSelectAdopter.c_str(), 65536, TRUE, nTimeOut, szErrorBuf ) )== NULL )
	{
		LOGBASE( ABNORMAL, _T("プロミスキャスモードでインターフェースオープン失敗 %s"), szErrorBuf);
		return;
	}

	LOGBASE( NORMAL, _T("プロミスキャスモードでインターフェースオープン %s"), m_strSelectAdopter.c_str());
}