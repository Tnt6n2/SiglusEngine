#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：時短再生
// ================================================================
class Ccfg_wnd_func_jitan : public C_cfg_wnd_func
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
		Cc_group	grp_jitan;
		Cc_static	stc_jitan_normal;
		Cc_static	stc_jitan_2bai;
		Cc_static	stc_jitan_3bai;
		Cc_slider	sld_jitan;
		Cc_check	chk_jitan_normal;
		Cc_check	chk_jitan_automode;
		Cc_check	chk_jitan_msgbk;
		Cc_button	btn_jitan_init;
		Cc_button	btn_jitan_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		update_dialog();			// ダイアログ更新
	void		update_text_info();			// テキスト情報の更新
};
