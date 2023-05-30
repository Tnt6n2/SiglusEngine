#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：オートモード
// ================================================================
class Ccfg_wnd_func_automode : public C_cfg_wnd_func
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
		Cc_group	grp_automode;
		Cc_static	stc_automode;
		Cc_group	grp_automode_moji_time;
		Cc_static	stc_automode_moji_time;
		Cc_slider	sld_automode_moji_time;
		Cc_group	grp_automode_min_time;
		Cc_static	stc_automode_min_time;
		Cc_slider	sld_automode_min_time;
		Cc_static	stc_automode_result_text;
		Cc_static	stc_automode_result;
		Cc_button	btn_automode_init;
		Cc_button	btn_automode_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログを更新
	void		update_text_info();			// テキスト情報の更新
};
