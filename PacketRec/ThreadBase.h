#ifndef __THREAD_BASE_H__
#define __THREAD_BASE_H__

#include "windows.h"

class CThreadBase
{
public:
	// コンストラクタ
	CThreadBase();
	
	// デストラクタ
	~CThreadBase();

	// 初期化
	virtual void Initialize();

	// スレッド作成
	void CreateThread( BOOL bStart = TRUE );

	// スレッド破棄
	BOOL Destroy( DWORD dwTimeOut = INFINITE );

	// スレッドメイン処理
	virtual UINT ThreadMain() = 0;

	// ハンドル取得
	HANDLE GetThreadHandle(){ return m_hThread; }

	// スレッド実行
	void StartThread();

protected:

	// スレッドのエントリポイント
	static UINT __stdcall ThreadEntry( void* pParam );

protected:
	// スレッドハンドル
	HANDLE m_hThread;
	
	// スレッドID
	UINT m_uThreadID;
};

#endif