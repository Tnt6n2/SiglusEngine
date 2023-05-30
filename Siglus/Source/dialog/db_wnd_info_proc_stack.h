#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：プロセススタック
// ================================================================
class C_db_wnd_info_proc_stack : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// アイテム
	struct	ITEM
	{
		TNM_PROC_TYPE		proc;
	};

	// ステータス
	struct STATE
	{
		ARRAY<ITEM>	item_list;
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

