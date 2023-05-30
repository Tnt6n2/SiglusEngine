#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ＢＧＭテーブル
// ================================================================
class C_db_wnd_info_bgm_table : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

	// ステータス
	struct STATE
	{
		ARRAY<int>	flag_list;
	}	m_ld;

private:
	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

