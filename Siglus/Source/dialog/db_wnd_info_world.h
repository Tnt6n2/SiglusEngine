#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ワールド
// ================================================================
class C_db_wnd_info_world : public C_db_wnd_info_listview
{
public:
	void		init();			// 初期化
	void		frame();		// フレーム

private:
	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション

	// ウィンドウプロシージャ
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知

	// リストビューをダブルクリックした
	void		on_dblclk_lv();

	// リストビューの値を変更した
	void		on_change_lv_value(int item_no, int column_no, CTSTR& value);
};

