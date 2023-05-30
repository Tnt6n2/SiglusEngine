#include	"pch.h"
#include	"tona3_main.h"
#include	"tnm_app.h"
#include	"..\resource.h"

#include	<atlbase.h>
#include	<atlhost.h>

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

static		CComModule	G_com_module;

// ****************************************************************
// メイン
// ================================================================
int tona_main()
{
	// アプリケーションの名前の設定（ini の保存などに使用します）
	G_app.app_name = _T("siglus_engine");

	// タイマーの精度を上げる
	TIMECAPS caps = {0};
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);

	// 乱数系列の初期化
	srand((UINT)time(NULL));

	// COM の初期化
	G_com_module.Init(ObjectMap, G_app.h_inst, &LIBID_ATLLib);

	// ATL を準備
	AtlAxWinInit();

	// Siglus アプリケーション
	{
		// 本体は大きすぎてスタックに乗りません。動的に作ります。
		BSP<C_tnm_app> tnm_app(new C_tnm_app);
		
		// アプリケーションのメイン
		tnm_app->main();
	}

	// COM モジュールを解放
	G_com_module.Term();

	// タイマーの制度を元に戻す
	timeEndPeriod(caps.wPeriodMin);

	// 正常終了
	return EXIT_SUCCESS;
}

