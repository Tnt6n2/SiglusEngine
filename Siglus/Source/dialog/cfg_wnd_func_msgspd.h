#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：文字速度
// ================================================================
class C_cfg_wnd_func_msgspd : public C_cfg_wnd_func
{
public:
	void		on_init_funcfunc();										// 初期化
	void		on_close_funcfunc();									// 閉じる
	bool		on_command_funcfunc(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify_funcfunc(WPARAM wp, LPARAM lp);				// 通知
	bool		on_mousewheel_funcfunc(WPARAM wp, LPARAM lp);			// マウスホイール
	bool		on_h_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｈスクロール
	bool		on_v_scroll_funcfunc(WPARAM wp, LPARAM lp);				// Ｖスクロール
	LRESULT		on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp);		// スタティックコントロールのカラー変更
	void		frame_funcfunc();										// フレーム

	void		init_state();							// 初期状態に戻す

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_group	grp_msgspd;
		Cc_slider	sld_msgspd;
		Cc_static	stc_msgspd_sample;
		Cc_static	stc_msgspd_slow;
		Cc_static	stc_msgspd_fast;
		Cc_check	chk_msgspd_nowait;
		Cc_button	btn_msgspd_init;
		Cc_button	btn_msgspd_close;
	}	ci;

	// 作業変数
	struct WORK
	{
		int			now_time;			// 現在の表示タイム
		TSTR		now_text;			// 現在の表示されている文字列
	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログ更新
	TSTR		get_sample_text();			// サンプルテキスト

	// 作業用フォント
	void		init_work_font();						// 初期化
	void		free_work_font();						// 開放
	void		load_work_font(CTSTR& font_name);		// 読み込み

	// 作業用フォント変数
	struct WORK_FONT
	{
		HFONT		original_font;		// 初期状態のフォント
		HFONT		now_font;			// 現在のフォント
		TSTR		now_font_name;		// 現在のフォントの名前
	}	work_font;

};
