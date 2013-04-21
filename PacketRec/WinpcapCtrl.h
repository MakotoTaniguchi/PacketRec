// WinpcapのAPI
#ifndef WINPCAP_H
#define WINPCAP_H

#include "pcap.h"
#include "windows.h"

#define MAX_RECV_SIZE	( 1024 )
#define FREAMSIZE		(18)

// アダプタ情報
typedef struct stDeviceData
{
	TSTRING strDeviceName;
	TSTRING strAdopter;
	TSTRING strIPAddress;
} CONNECTDEVICEDATA, *LP_CONNECTDEVICEDATA;

// アダプタリスト
typedef std::vector<CONNECTDEVICEDATA> VEC_DEVICEDATA, *LP_VEC_DEVICEDATA;

// フィルタタイプ
typedef struct FilterData
{
	TSTRING strSrcHost;
	TSTRING strDstHost;
	TSTRING strSrcIP;
	TSTRING strDstIP;
	TSTRING strSrcPort;
	TSTRING strDstPort;

	FilterData()
	{
		strSrcHost	= _T("");
		strDstHost	= _T("");
		strSrcIP	= _T("");
		strDstIP	= _T("");
		strSrcPort	= _T("");
		strDstPort	= _T("");
	}
} FILTERDATA, *LP_FILTERDATA;

// WinpcapのAPIを使用するクラス
class CWinpcapCtrl
{
public:
	// コンストラクタ
	CWinpcapCtrl();

	// デストラクタ
	~CWinpcapCtrl();

	// アダプタを収集
	static VEC_DEVICEDATA AdopterOpen();

	// キャプチャ開始
	void CaptureStart( LPTSTR lpzFilePath );

	// キャプチャ停止
	void CaptureStop( DWORD dwWaite = INFINITE );

	// 初期化
	void Initialize( LPTSTR lpzAdopterName );

	// パケットフィルタリング
	void PacketFiltering( FILTERDATA stFilterData );

private:

	// パケット用文字列作成
	LPTSTR MakeFilterChar( TSTRING strSrc, TSTRING strDst, TSTRING strType );

private:
	
	// アダプタ名
	TSTRING m_strSelectAdopter;

	// オブジェクト識別子
	pcap_t* m_pPcap;

	// キャンセルフラグ
	BOOL m_bCancel;

	// キャンセルイベント
	HANDLE m_hCancel;
};

#endif