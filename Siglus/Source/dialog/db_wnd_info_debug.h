#pragma		once

#include	"dialog/db_wnd_info.h"

// ****************************************************************
// デバッグ情報ウィンドウ：デバッグ設定
// ================================================================
class C_db_wnd_info_debug : public C_db_wnd_info
{

public:

	void		init();						// 初期化
	void		load_state();				// ステータス読み込み
	void		save_state();				// ステータス保存
	void		frame();					// フレーム

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_static	stc_ss;
		Cc_edit		edt_ss;
		Cc_static	stc_editor;
		Cc_edit		edt_editor;
		Cc_static	stc_koe;
		Cc_edit		edt_koe;
		Cc_static	stc_koe_tool;
		Cc_edit		edt_koe_tool;
		Cc_button	btn_open;
	}	ci;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド

	bool		on_edit_ss_path();
	bool		on_edit_editor_path();
	bool		on_edit_koe_path();
	bool		on_edit_koe_tool_path();
	bool		on_btn_open();
};


