#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ＣＧテーブル
// ================================================================
class C_db_wnd_info_cg_table : public C_db_wnd_info
{
public:
	void		init();					// 初期化
	void		load_state();			// ステータス読み込み
	void		save_state();			// ステータス保存
	void		frame();				// フレーム

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_tab				tab;
		C_tnm_dbg_listview	lst_original;
		C_tnm_dbg_listview	lst_group;
	}	ci;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	void		on_dblclk_lv(int ctrl_id);						// リストビューをダブルクリックした

	void		change_page(int page_no);							// ページを変更する
	void		change_flag(int ctrl_id, int item_no, int value);	// フラグを更新する

	C_tnm_dbg_listview*	get_lv_by_ctrl_id(int ctrl_id);			// コントロールＩＤからリストビューを取得する
};

