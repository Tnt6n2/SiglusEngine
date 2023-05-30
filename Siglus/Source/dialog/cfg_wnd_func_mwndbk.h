#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：ウィンドウ背景色
// ================================================================
class Ccfg_wnd_func_mwndbk : public C_cfg_wnd_func
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
		Cc_group	grp_mwndbk;
		Cc_static	stc_mwndbk_r;
		Cc_slider	sld_mwndbk_r;
		Cc_static	stc_mwndbk_g;
		Cc_slider	sld_mwndbk_g;
		Cc_static	stc_mwndbk_b;
		Cc_slider	sld_mwndbk_b;
		Cc_static	stc_mwndbk_tr;
		Cc_slider	sld_mwndbk_tr;
		Cc_button	btn_mwndbk_init;
		Cc_button	btn_mwndbk_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		update_dialog();			// ダイアログ更新
};
