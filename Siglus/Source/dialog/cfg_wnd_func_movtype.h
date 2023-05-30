#pragma		once

#include	"cfg_wnd_func.h"

// ****************************************************************
// コンフィグウィンドウ：ファンクション：システム
// ================================================================
class Ccfg_wnd_func_movtype : public C_cfg_wnd_func
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

	bool		support_return_value;					// ムービーサポート用の返り値

private:

	// コントロールアイテム
	struct CONTROL_ITEM
	{
		Cc_static	stc_movtype_mess00;
		Cc_static	stc_movtype_mess01;
		Cc_radio	rad_movtype_mci;
		Cc_radio	rad_movtype_wmp;
		Cc_button	btn_movtype_close;
		Cc_button	btn_movtype_deside;
		Cc_button	btn_movtype_cancel;
	}	ci;

	// 作業変数
	struct WORK
	{
	}	work;

	void		update_dialog();			// ダイアログ更新

	void		set_radio_movtype();		// ムービーの再生方法のラジオボタンを設定
	int			get_radio_movtype();		// ムービーの再生方法のラジオボタンを取得
};
