#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：音声モード
// ================================================================
class Ccfg_wnd_func_koemode : public C_cfg_wnd_func
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
		Cc_group	grp_koemode;
		Cc_radio	rad_koemode_normal;
		Cc_radio	rad_koemode_textonly;
		Cc_radio	rad_koemode_koeonly;
		Cc_button	btn_koemode_init;
		Cc_button	btn_koemode_close;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		setup_dialog();				// ダイアログを構築
	void		update_dialog();			// ダイアログ更新
	void		set_radio_koemode();		// 声モードのラジオボタンを設定
	int			get_radio_koemode();		// 声モードのラジオボタンを取得

};
